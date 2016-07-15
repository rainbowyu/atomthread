#include "myos.h"
#include "myosdisplay.h"
#include "stm8sdriver.h"
#ifdef MYOS_DISPLAYMODE



/************************************************************************信号引脚宏定义************************************************************************/
#define CS_HIGH         GPIOE->ODR |= 0x08                                      //片选线高电平
#define CS_LOW          GPIOE->ODR &= 0xF7                                      //片选线低电平

#define CD_DATA         GPIOE->ODR |= 0x80                                      //数据状态选通
#define CD_COMMAND      GPIOE->ODR &= 0x7F                                      //命令状态选通

#define RESET_HIGH      GPIOE->ODR |= 0x40                                      //复位线高电平
#define RESET_LOW       GPIOE->ODR &= 0xBF                                      //复位线低电平



/*********************************************************************显示模块驱动私有函数*********************************************************************/
/*
*函数名：LCD_Command(unsigned char8 command)
*入参：
  command：写入显示屏的命令
*返回值：无
*作用域：本地
*线程访问：回调
*函数作用：向LCD写入命令
*/
static void LCD_Command(unsigned char8 command)
{
  CD_COMMAND;                                                                   //命令模式
  MCU_SPISend(command);                                                         //写入命令
}



/***********************************************************************显示模块驱动函数***********************************************************************/
/*
*函数名：MCU_DisplayInit( void )
*入参：无
*返回值：无
*作用域：本地
*线程访问：回调
*函数作用：初始化硬件显示模块
*/
void MCU_DisplayInit( void )
{
  CS_LOW;                                                                       //进行一次硬件复位
  RESET_LOW;
  OS_Delay(0x8000);
  RESET_HIGH;
  OS_Delay(0x0800);

  LCD_Command(0xE2);                                                            //进行一次软件复位
  OS_Delay(0x0800);
  LCD_Command(0x2C);
  OS_Delay(0x0800);
  LCD_Command(0x2E);
  OS_Delay(0x0800);
  LCD_Command(0x2F);
  OS_Delay(0x0800);                                                             //配置升压参数

  LCD_Command(0x24);
  LCD_Command(0x81);
  LCD_Command(0x1A);                                                            //0x1a,微调对比度的值，可设置范围0x00～0x3f

  LCD_Command(0xa2);
  LCD_Command(0xc8);
  LCD_Command(0xa0);
  LCD_Command(0x40);
  LCD_Command(0xAF);                                                            //开显示

  CS_HIGH;                                                                      //拉高片选,暂不使用LCD模块
}

/*
*函数名：MCU_SetCursor(unsigned short16 x, unsigned short16 y)
*入参：
  x：X显示坐标
  y：Y显示坐标
*返回值：无
*作用域：本地
*线程访问：回调
*函数作用：设置显示的坐标
*/
void MCU_SetCursor(unsigned short16 x, unsigned short16 y)
{
  CS_LOW;                                                                       //拉低片选,使能LCD模块
  y &= 0x07;
  x |= 0x0100;
  LCD_Command(y | 0xB0);                                                        //设置页地址,每页是8行,一个画面的64行被分成8个页
  LCD_Command((x >> 0x04) & 0x001F);                                            //设置列地址的高4位
  LCD_Command(x & 0x000F);                                                      //设置列地址的低4位
}

/*
*函数名：MCU_WriteRAM_Start( void )
*入参：无
*返回值：无
*作用域：本地
*线程访问：回调
*函数作用：设置写入GRAM开始
*/
void MCU_WriteRAM_Start( void )
{
}

/*
*函数名：MCU_WriteRAM_Stop( void )
*入参：无
*返回值：无
*作用域：本地
*线程访问：回调
*函数作用：结束数据操作
*/
void MCU_WriteRAM_Stop( void )
{
  CS_HIGH;
}

/*
*函数名：MCU_WriteRAM(unsigned short16 data)
*入参：
  data：写入显示屏的显示数据
*返回值：无
*作用域：本地
*线程访问：回调
*函数作用：向显示屏中写入数据
*/
void MCU_WriteRAM(unsigned short16 data)
{
  CD_DATA;                                                                      //数据模式
  MCU_SPISend(data);                                                            //写入数据
}

/*
*函数名：MCU_DrawVerticalLine(unsigned short16 x, unsigned short16 y, unsigned short16 length, unsigned short16 color)
*入参：
  x：线的起始X坐标
  y：线的起始Y坐标
  length：线的长度
  color：线的绘制颜色
*返回值：无
*作用域：本地
*线程访问：回调
*函数作用：在显示屏上向下画一条竖线
*/
void MCU_DrawVerticalLine(unsigned short16 x, unsigned short16 y, unsigned short16 length, unsigned short16 color)
{
  unsigned char8 mask = 0xFF;                                                   //数据掩码
  unsigned char8 regist = 0x00;                                                 //运算寄存
  if(length == 0x00){return ;}                                                  //入参检查
  
  if(length < 0x08){mask >>= (0x08 - length);}                                  //长度检查
  
  regist = (y & 0x07);                                                          //获取偏移
  length = (length > (0x08 - regist)) ? (length - (0x08 - regist)) : 0x00;      //长度自减
  regist = (mask << regist);                                                    //获取掩码
  
  y >>= 0x03;
  OS_SetCursor(x,y);                                                            //设置坐标
  OS_WriteRAM_Start();                                                          //开始写入
  OS_WriteRAM(regist);                                                          //写入数据
  
  while(length > 0x08){                                                         //长度检查
    OS_SetCursor(x,(++ y));                                                     //设置坐标
    OS_WriteRAM_Start();                                                        //开始写入
    OS_WriteRAM(0xFF);                                                          //写入数据
    length -= 0x08;                                                             //写入遍历
  }
  
  OS_SetCursor(x,(++ y));                                                       //设置坐标
  OS_WriteRAM_Start();                                                          //开始写入
  regist = (0xFF >> (0x08 - length));                                           //获取掩码
  OS_WriteRAM(regist);                                                          //写入数据
  
  OS_WriteRAM_Stop();
}

/*
*函数名：MCU_DrawHorizontalLine(unsigned short16 x, unsigned short16 y, unsigned short16 length, unsigned short16 color)
*入参：
  x：线的起始X坐标
  y：线的起始Y坐标
  length：线的长度
  color：线的绘制颜色
*返回值：无
*作用域：本地
*线程访问：回调
*函数作用：在显示屏上向右画一条横线
*/
void MCU_DrawHorizontalLine(unsigned short16 x, unsigned short16 y, unsigned short16 length, unsigned short16 color)
{
}

/*
*函数名：MCU_GetExternFont(unsigned short16 size, unsigned int32 start, unsigned char8 *regbuff)
*入参：
  size：字体在主存中的数据大小
  start：字体在主存中的开始地址
  regbuff：字体数据缓存数组
*返回值：是否读取成功
*作用域：本地
*线程访问：回调
*函数作用：从主存中读取字体数据
*/
unsigned char8 MCU_GetExternFont(unsigned short16 size, unsigned int32 start, unsigned char8 *regbuff)
{
  if(start == 0x00 || regbuff == 0x00 || size == 0x00){return 0x00;}            //入参检查
  return 0x00;
}

#endif

