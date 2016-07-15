#include "myos.h"
#include "stm8sdriver.h"


extern const Myos_CoreData volatile Myos_Core0 __at(CONFIGURE_CORE0);



/****************************************************************STM8S系列微处理器通用驱动函数*****************************************************************/
/*
*函数名：MCU_FrequencyInit( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化时钟系统
*/
void MCU_FrequencyInit( void )
{
  CLK->ICKR = 0x29;                                                             //活跃停机模式下主电压调节器开、低速内部振荡器开、高速内部振荡器开
  while((CLK->ICKR & 0x12) != 0x12);                                            //等待振荡器就绪

  CLK->SWR     = 0xE1;                                                          //设置高速内部振荡器为主时钟源
  CLK->ECKR    = 0x00;                                                          //外部振荡器关
  CLK->CSSR    = 0x00;                                                          //时钟安全系统关
  CLK->SWCR    = 0x00;                                                          //清除时钟源切换标志及关闭时钟切换中断
  CLK->CKDIVR  = 0x00;                                                          //主时钟不分频,全速运行
  CLK->PCKENR1 = 0xFF;                                                          //打开所有外设时钟
  CLK->PCKENR2 = 0xFF;                                                          //打开所有外设时钟
}

/*
*函数名：MCU_ExitInterruptInit( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化外部中断触发方式
*/
void MCU_ExitInterruptInit( void )
{
  EXTI->CR1 = 0x55;                                                             //外部中断触发设置
  EXTI->CR2 = 0x55;                                                             //外部中断触发设置
}

/*
*函数名：MCU_GPIOAInit( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化GPIOA端口
*/
void MCU_GPIOAInit( void )
{
  GPIOA->ODR = 0x00;                                                            //GPIOA端口数据输出设置
  GPIOA->DDR = 0x08;                                                            //GPIOA端口数据方向设置
  GPIOA->CR1 = 0x08;                                                            //GPIOA端口属性配置
  GPIOA->CR2 = 0x00;                                                            //GPIOA端口属性配置
}

/*
*函数名：GPIOA_Interrupt( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：GPIOA端口的中断处理函数
*/
#pragma vector=0x05
__interrupt void GPIOA_Interrupt( void )
{
}

/*
*函数名：MCU_GPIOBInit( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化GPIOB端口
*/
void MCU_GPIOBInit( void )
{
  GPIOB->ODR = 0x00;                                                            //GPIOB端口数据输出设置
  GPIOB->DDR = 0x00;                                                            //GPIOB端口数据方向设置
  GPIOB->CR1 = 0x00;                                                            //GPIOB端口属性配置
  GPIOB->CR2 = 0x00;                                                            //GPIOB端口属性配置
}

/*
*函数名：GPIOB_Interrupt( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：GPIOB端口的中断处理函数
*/
#pragma vector=0x06
__interrupt void GPIOB_Interrupt( void )
{
}

/*
*函数名：MCU_GPIOCInit( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化GPIOC端口
*/
void MCU_GPIOCInit( void )
{
  GPIOC->ODR = 0x00;                                                            //GPIOC端口数据输出设置
  GPIOC->DDR = 0x00;                                                            //GPIOC端口数据方向设置
  GPIOC->CR1 = 0x00;                                                            //GPIOC端口属性配置
  GPIOC->CR2 = 0x00;                                                            //GPIOC端口属性配置
}

/*
*函数名：GPIOC_Interrupt( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：GPIOC端口的中断处理函数
*/
#pragma vector=0x07
__interrupt void GPIOC_Interrupt( void )
{
}

/*
*函数名：MCU_GPIODInit( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化GPIOD端口
*/
void MCU_GPIODInit( void )
{
  GPIOD->ODR = 0x00;                                                            //GPIOD端口数据输出设置
  GPIOD->DDR = 0x20;                                                            //GPIOD端口数据方向设置
  GPIOD->CR1 = 0x20;                                                            //GPIOD端口属性配置
  GPIOD->CR2 = 0x20;                                                            //GPIOD端口属性配置
}

/*
*函数名：GPIOD_Interrupt( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：GPIOD端口的中断处理函数
*/
#pragma vector=0x08
__interrupt void GPIOD_Interrupt( void )
{
}

/*
*函数名：MCU_GPIOEInit( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化GPIOE端口
*/
void MCU_GPIOEInit( void )
{
  GPIOE->ODR = 0x01;                                                            //GPIOE端口数据输出设置
  GPIOE->DDR = 0xC9;                                                            //GPIOE端口数据方向设置
  GPIOE->CR1 = 0xC9;                                                            //GPIOE端口属性配置
  GPIOE->CR2 = 0x00;                                                            //GPIOE端口属性配置
}

/*
*函数名：GPIOE_Interrupt( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：GPIOE端口的中断处理函数
*/
#pragma vector=0x09
__interrupt void GPIOE_Interrupt( void )
{
}

/*
*函数名：MCU_GPIOFInit( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化GPIOF端口
*/
void MCU_GPIOFInit( void )
{
  GPIOF->ODR = 0x00;                                                            //GPIOF端口数据输出设置
  GPIOF->DDR = 0x00;                                                            //GPIOF端口数据方向设置
  GPIOF->CR1 = 0x00;                                                            //GPIOF端口属性配置
  GPIOF->CR2 = 0x00;                                                            //GPIOF端口属性配置
}

/*
*函数名：GPIOF_Interrupt( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：GPIOF端口的中断处理函数
*/
#pragma vector=0x0A
__interrupt void GPIOF_Interrupt( void )
{
}

/*
*函数名：MCU_GPIOGInit( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化GPIOG端口
*/
void MCU_GPIOGInit( void )
{
  GPIOG->ODR = 0x02;                                                            //GPIOG端口数据输出设置
  GPIOG->DDR = 0x02;                                                            //GPIOG端口数据方向设置
  GPIOG->CR1 = 0x02;                                                            //GPIOG端口属性配置
  GPIOG->CR2 = 0x00;                                                            //GPIOG端口属性配置
}

/*
*函数名：MCU_FlashInit( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化设备的存储器
*/
void MCU_FlashInit( void )
{
  FLASH->CR1   = 0x00;                                                          //停机下关闭存储器
  FLASH->CR2   = 0x00;                                                          //系统存储器编程禁止
  FLASH->NCR2  = 0xFF;                                                          //CR2的互补寄存器
  FLASH->IAPSR = 0x00;                                                          //锁定存储器写保护
}

/*
*函数名：MCU_OptionSet(unsigned char8 option, unsigned char8 noption, unsigned char8 value)
*入参：
  option：选项字节序号
  noption：选项字节是否有互补字节
  value：需要设置的值
*返回值：无
*作用域：本地
*函数作用：设置选项字节中的值
*/
void MCU_OptionSet(unsigned char8 option, unsigned char8 noption, unsigned char8 value)
{
  unsigned char8 timeout = 0xFF;
  unsigned char8 *address = ((void *)(option + 0x4800));                        //计算选项字节的存储基地址
  FLASH->CR2  = 0x80;                                                           //使能对选项字节的编程
  FLASH->NCR2 = 0x7F;                                                           //使能对选项字节的编程

  FLASH->DUKR = 0xAE;                                                           //写入第一个秘钥
  FLASH->DUKR = 0x56;                                                           //写入第二个秘钥
  while(((FLASH->IAPSR) & 0x08) == 0x00 && (-- timeout) > 0x00);                //等待解密就绪

  *(address ++) = value;                                                        //写入配置数据
  if(noption > 0x00){*address = (~value);}                                      //写入互补数据
  FLASH->IAPSR = 0x00;                                                          //重新锁定写保护
  FLASH->CR2  = 0x00;                                                           //禁止对选项字节的编程
  FLASH->NCR2 = 0xFF;                                                           //禁止对选项字节的编程
}

/*
*函数名：MCU_ChipSerial(unsigned char8 *cache)
*入参：
  cache：芯片序列号的缓存地址
*返回值：无
*作用域：本地
*函数作用：获取芯片的唯一序列号
*/
void MCU_ChipSerial(unsigned char8 *cache)
{
  unsigned char8 count = 0x07;
  unsigned char8 *address = ((void *)(0x48D2));                                 //芯片序列号基地址计算
  if(cache == 0x00){return ;}                                                   //入参检查
  while((count --) > 0x00){*(cache ++) = *(address ++);}                        //读取芯片序列号
}

/*
*函数名：MCU_EEPROMWrite(unsigned short16 offset, unsigned short16 length, unsigned char8 *cache)
*入参：
  offset：写入的偏移数
  length：写入的数据长度
  cache：写入的数据缓存
*返回值：无
*作用域：本地
*函数作用：向EEPROM中写入数据
*/
void MCU_EEPROMWrite(unsigned short16 offset, unsigned short16 length, unsigned char8 *cache)
{
  unsigned char8 timeout = 0xFF;
  unsigned char8 *address = ((void *)(offset + 0x4000));                        //计算数据的EEPROM存储基地址
  if(cache == 0x00){return ;}                                                   //入参检查

  FLASH->DUKR = 0xAE;                                                           //写入第一个秘钥
  FLASH->DUKR = 0x56;                                                           //写入第二个秘钥
  while(((FLASH->IAPSR) & 0x08) == 0x00 && (-- timeout) > 0x00);                //等待解密就绪

  while((length --) > 0x00){*(address ++) = *(cache ++);}                       //数据写入
  FLASH->IAPSR = 0x00;                                                          //重新锁定写保护
}

/*
*函数名：MCU_EEPROMRead(unsigned short16 offset, unsigned short16 length, unsigned char8 *cache)
*入参：
  offset：读取的偏移数
  length：读取的数据大小
  cache：读取的数据缓存
*返回值：无
*作用域：本地
*函数作用：从EEPROM中读取数据
*/
void MCU_EEPROMRead(unsigned short16 offset, unsigned short16 length, unsigned char8 *cache)
{
  unsigned char8 *address = ((void *)(offset + 0x4000));                        //计算数据的EEPROM存储基地址
  if(cache == 0x00){return ;}                                                   //入参检查
  while((length --) > 0x00){*(cache ++) = *(address ++);}                       //读取数据
}

/*
*函数名：MCU_BEEPInit( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化蜂鸣器控制器
*/
void MCU_BEEPInit( void )
{
//  MCU_OptionSet(0x03,0x01,0x80);                                                //是否允许BEEP输出
  BEEP->CSR = 0x4E;                                                             //初始化为BEEP频率为2KHZ
}

/*
*函数名：MCU_BEEPSet(unsigned char8 status)
*入参：
  status：状态变量
*返回值：无
*作用域：本地
*函数作用：设置蜂鸣器的状态
*/
void MCU_BEEPSet(unsigned char8 status)
{
  if(status > 0x00){BEEP->CSR |= 0x20;}else{BEEP->CSR &= 0xDF;}                 //设置蜂鸣器的状态
}

/*
*函数名：MCU_SPIInit( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化SPI通信接口
*/
void MCU_SPIInit( void )
{
  SPI->SR  = 0x00;                                                              //清空通信标志位
//  SPI->ICR = 0x40;                                                              //打开SPI的接收完成中断
  SPI->CR2 = 0x03;                                                              //配置SPI为双线单向,设置从设备使能为软件控制,内部使能设置为主设备
  SPI->CR1 = 0x4C;                                                              //设置SPI为MSB先发,时钟四分频,配置为主设备,空闲为低电平从第一个时钟沿采样数据,同时启动SPI运行
}

/*
*函数名：MCU_SPIRead(unsigned char8 data)
*入参：
  data：需要回复的数据
*返回值：主机发送的数据及是否超时
*作用域：本地
*函数作用：读取SPI接收到的数据并在下一个通信周期回复主机数据
*/
unsigned short16 MCU_SPIRead(unsigned char8 data)
{
  unsigned short16 regist = 0xFF;
  while((SPI->SR & 0x01) == 0x00 && (-- regist) > 0x00);                        //判断是否接收完成
  regist = ((regist << 0x08) | SPI->DR);                                        //读取收到的数据
  SPI->DR = data;                                                               //将数据写入SPI缓冲
  return regist;
}

/*
*函数名：MCU_SPISend(unsigned char8 data)
*入参：
  data：需要发送的数据
*返回值：从机回复的数据及是否超时
*作用域：本地
*函数作用：通过SPI发送数据并接受从机返回的数据
*/
unsigned short16 MCU_SPISend(unsigned char8 data)
{
  unsigned short16 regist = 0xFF;
  SPI->DR = data;                                                               //将数据写入SPI缓冲
  while((SPI->SR & 0x01) == 0x00 && (-- regist) > 0x00);                        //判断是否接收完成
  regist = ((regist << 0x08) | SPI->DR);                                        //读取收到的数据
  return regist;
}

/*
*函数名：SPI_Interrupt( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：SPI通信接口的中断处理函数
*/
#pragma vector=0x0C
__interrupt void SPI_Interrupt( void )
{
  unsigned char8 regist = SPI->DR;                                              //清空传输完成标志
  SPI->SR = (regist & 0x00);                                                    //清空状态标识位
}

/*
*函数名：MCU_TIM1Init( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化TIM1定时器
*/
void MCU_TIM1Init( void )
{
  TIM1->SR1 = 0x00;                                                             //清空事件标识位
  TIM1->SR2 = 0x00;                                                             //清空捕获事件标志位

  TIM1->CR1 = 0x80;                                                             //开启预装载
  TIM1->CR2 = 0x00;                                                             //不作为主定时器使用

  TIM1->PSCRH = 0x00;                                                           //设置高位分频器
  TIM1->PSCRL = 0x0F;                                                           //设置低位分频器

  TIM1->CNTRH = 0x00;                                                           //清空高位计时器
  TIM1->CNTRL = 0x00;                                                           //清空低位计时器

  TIM1->ARRH = 0x03;                                                            //设置高位预装载寄存器
  TIM1->ARRL = 0xE8;                                                            //设置低位预装载寄存器

  TIM1->CCR1H = 0x00;                                                           //设置通道1占空比
  TIM1->CCR1L = 0x01;                                                           //设置通道1占空比

  TIM1->CCR2H = 0x00;                                                           //设置通道2占空比
  TIM1->CCR2L = 0x02;                                                           //设置通道2占空比

  TIM1->CCR3H = 0x00;                                                           //设置通道3占空比
  TIM1->CCR3L = 0x03;                                                           //设置通道3占空比

  TIM1->CCR4H = 0x00;                                                           //设置通道4占空比
  TIM1->CCR4L = 0x04;                                                           //设置通道4占空比

  TIM1->CCMR1 = 0x68;                                                           //设置通道1为PWM1模式,开始预装载使能并设置通道为输出模式
  TIM1->CCMR2 = 0x68;                                                           //设置通道2为PWM1模式,开始预装载使能并设置通道为输出模式
  TIM1->CCMR3 = 0x68;                                                           //设置通道3为PWM1模式,开始预装载使能并设置通道为输出模式
  TIM1->CCMR4 = 0x68;                                                           //设置通道4为PWM1模式,开始预装载使能并设置通道为输出模式

  TIM1->CCER1 = 0x33;                                                           //设置通道1、2的输出使能,半字节的最低位启用,并开启反相输出,即低电平有效
  TIM1->CCER2 = 0x33;                                                           //设置通道3、4的输出使能,半字节的最低位启用,并开启反相输出,即低电平有效

  TIM1->BKR = 0x80;                                                             //设置主输出使能
  TIM1->IER = 0x43;                                                             //中断控制寄存器,允许更新中断和比较中断,比较中断源设置为仅通道1
  TIM1->CR1 = 0x81;                                                             //开启预装载,边沿向上计数,允许发生更新事件并启动定时器
}

/*
*函数名：TIM1_Interrupt( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：TIM1的更新中断处理函数
*/
#pragma vector=0x0D
__interrupt void TIM1_Interrupt( void )
{
  TIM1->SR1 = 0x00;                                                             //清空标志位
}

/*
*函数名：TIM1_CompareInterrupt( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：TIM1的比较中断处理函数
*/
#pragma vector=0x0E
__interrupt void TIM1_CompareInterrupt( void )
{
  TIM1->SR1 = 0x00;                                                             //清空标志位
}

/*
*函数名：MCU_TIM2Init( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化TIM2定时器
*/
void MCU_TIM2Init( void )
{
  TIM2->SR1 = 0x00;                                                             //清空事件标识位
  TIM2->SR2 = 0x00;                                                             //清空捕获事件标志位

  TIM2->CR1 = 0x80;                                                             //开启预装载

  TIM2->PSCR = 0x04;                                                            //设置分频器

  TIM2->CNTRH = 0x00;                                                           //清空高位计时器
  TIM2->CNTRL = 0x00;                                                           //清空低位计时器

  TIM2->ARRH = 0xFF;                                                            //设置高位预装载寄存器
  TIM2->ARRL = 0xFF;                                                            //设置低位预装载寄存器

  TIM2->CCR1H = 0x00;                                                           //清空通道1捕获值
  TIM2->CCR1L = 0x00;                                                           //清空通道1捕获值

  TIM2->CCR2H = 0x00;                                                           //清空通道2捕获值
  TIM2->CCR2L = 0x00;                                                           //清空通道2捕获值

  TIM2->CCR3H = 0x03;                                                           //设置通道3的占空比
  TIM2->CCR3L = 0xE8;                                                           //设置通道3的占空比

  TIM2->CCMR1 = 0x31;                                                           //设置通道1为捕获模式,设置为8次采样滤波并映射到捕获通道1节点1
  TIM2->CCMR2 = 0x32;                                                           //设置通道2为捕获模式,设置为8次采样滤波并映射到捕获通道1节点2
  TIM2->CCMR3 = 0x68;                                                           //设置通道3为PWM1模式,开始预装载使能并设置通道为输出模式

  TIM2->CCER1 = 0x31;                                                           //设置通道1为上升沿捕获、通道2为下降沿捕获
  TIM2->CCER2 = 0x01;                                                           //设置通道3输出使能、通道4的输出禁止

  TIM2->IER = 0x43;                                                             //中断控制寄存器,允许更新中断和捕获中断,捕获通道设置为捕获通道1
  TIM2->CR1 = 0x81;                                                             //开启预装载,边沿向上计数,允许发生更新事件并启动定时器
}

/*
*函数名：TIM2_Interrupt(void)
*入参：无
*返回值：无
*作用域：本地
*函数作用：TIM2的更新中断处理函数
*/
#pragma vector=0x0F
__interrupt void TIM2_Interrupt(void)
{
  TIM2->SR1 = 0x00;                                                             //清空标志位
}

/*
*函数名：TIM2_CompareInterrupt(void)
*入参：无
*返回值：无
*作用域：本地
*函数作用：TIM2的捕获中断处理函数
*/
#pragma vector=0x10
__interrupt void TIM2_CompareInterrupt(void)
{
  TIM2->SR1 = 0x00;                                                             //清空标志位
}

/*
*函数名：MCU_TIM3Init( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化TIM3定时器
*/
void MCU_TIM3Init( void )
{
  TIM3->SR1 = 0x00;                                                             //清空事件标识位
  TIM3->SR2 = 0x00;                                                             //清空捕获事件标志位

  TIM3->CR1 = 0x80;                                                             //开启预装载

  TIM3->PSCR = 0x04;                                                            //设置分频器

  TIM3->CNTRH = 0x00;                                                           //清空高位计时器
  TIM3->CNTRL = 0x00;                                                           //清空低位计时器

  TIM3->ARRH = 0x27;                                                            //设置高位预装载寄存器
  TIM3->ARRL = 0x10;                                                            //设置低位预装载寄存器

  TIM3->CCR1H = 0x03;                                                           //设置通道1的占空比
  TIM3->CCR1L = 0xE8;                                                           //设置通道1的占空比

  TIM3->CCR2H = 0x00;                                                           //设置通道2的占空比
  TIM3->CCR2L = 0x00;                                                           //设置通道2的占空比

  TIM3->CCMR1 = 0x68;                                                           //设置通道1为PWM1模式,开始预装载使能并设置通道为输出模式
  TIM3->CCMR2 = 0x68;                                                           //设置通道2为PWM1模式,开始预装载使能并设置通道为输出模式

  TIM3->CCER1 = 0x00;                                                           //设置通道1、2禁止输出

  TIM3->IER = 0x43;                                                             //中断控制寄存器,允许更新中断和捕获中断,捕获通道设置为捕获通道1
  TIM3->CR1 = 0x81;                                                             //开启预装载,边沿向上计数,允许发生更新事件并启动定时器
}

/*
*函数名：TIM3_Interrupt(void)
*入参：无
*返回值：无
*作用域：本地
*函数作用：TIM3的更新中断处理函数
*/
#pragma vector=0x11
__interrupt void TIM3_Interrupt(void)
{
  TIM3->SR1 = 0x00;                                                             //清空标志位
  OS_ComUpdate();                                                               //通信组件时钟基准
}

/*
*函数名：TIM3_CompareInterrupt(void)
*入参：无
*返回值：无
*作用域：本地
*函数作用：TIM3的比较中断处理函数
*/
#pragma vector=0x12
__interrupt void TIM3_CompareInterrupt(void)
{
  TIM3->SR1 = 0x00;                                                             //清空标志位
}

/*
*函数名：MCU_UART1Init(unsigned int32 baudrate)
*入参：
  baudrate：串口波特率
*返回值：无
*作用域：本地
*函数作用：初始化UART1通信接口
*/
void MCU_UART1Init(unsigned int32 baudrate)
{
  unsigned short16 regist = (((CLK->CKDIVR) >> 0x03) & 0x03);                   //获取主时钟的分频系数

  switch (regist){                                                              //计算当前的分频系数
    case (0x00):{regist = 0x01;break;}                                          //不分频
    case (0x01):{regist = 0x02;break;}                                          //二分频
    case (0x02):{regist = 0x04;break;}                                          //四分频
    case (0x03):{regist = 0x08;break;}                                          //八分频
    default:{regist = 0x08;break;}                                              //未定义状态统一默认为八分频
  }
  regist = 16000000 / (baudrate * regist);                                      //计算实际的波特率

  UART1->BRR2 = (((regist >> 0x08) & 0xF0) | (regist & 0x0F));                  //先设置波特率控制寄存器BRR2
  UART1->BRR1 = ((regist >> 0x04) & 0xFF);                                      //后设置波特率控制寄存器BRR1

  UART1->SR   = 0x00;                                                           //清空通信标志位
  UART1->CR2  = 0x2C;                                                           //开启发送和接收,并打开接收中断
  UART1->CR3  = 0x00;                                                           //设置为1个停止位,不启用同步时钟
  UART1->CR4  = 0x00;                                                           //不启用LIN功能
  UART1->CR5  = 0x00;                                                           //不启用智能卡和红外模式
  UART1->GTR  = 0x00;                                                           //使用默认智能卡保护时间
  UART1->PSCR = 0x00;                                                           //不使用智能卡和红外分频
  UART1->CR1  = 0x00;                                                           //使能UART并禁用校验功能
}

/*
*函数名：MCU_UART1Send(unsigned char8 data)
*入参：
  data：需要发送的数据
*返回值：是否等待超时
*作用域：本地
*函数作用：通过UART1发送一个数据
*/
unsigned char8 MCU_UART1Send(unsigned char8 data)
{
  unsigned char8 regist = 0xFF;                                                 //等待超时变量
  while(((UART1->SR & 0x80) == 0x00) && (-- regist) > 0x00);                    //等待发送缓存空
  UART1->DR = data;                                                             //将数据写入发送缓存
  return regist;
}

/*
*函数名：MCU_UART1Read( void )
*入参：无
*返回值：读取到的数据及是否超时
*作用域：本地
*函数作用：读取UART1收到的数据
*/
unsigned short16 MCU_UART1Read( void )
{
  unsigned short16 regist = 0xFF;                                               //等待超时变量
  while(((UART1->SR & 0x20) == 0x00) && (-- regist) > 0x00);                    //等待数据接收完成
  regist = ((regist << 0x08) | UART1->DR);                                      //读出收到的数据
  return regist;
}

/*
*函数名：UART1_TxInterrupt( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：UART1数据发送完成中断
*/
#pragma vector=0x13
__interrupt void UART1_TxInterrupt( void )
{
}

/*
*函数名：UART1_RxInterrupt( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：UART1数据接收完成中断
*/
#pragma vector=0x14
__interrupt void UART1_RxInterrupt( void )
{
  MCU_UART1Read();                                                              //读取接收到的数据
}

/*
*函数名：MCU_IICInit( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化IIC通信接口
*/
void MCU_IICInit( void )
{
  I2C->CR2    = 0x00;                                                           //清空复位标识
  I2C->SR1    = 0x00;                                                           //清空通信标志
  I2C->SR2    = 0x00;                                                           //清空错误标志
  I2C->SR3    = 0x00;                                                           //清空状态标志
//  I2C->ITR    = 0x06;                                                           //打开IIC的事件中断

  I2C->CCRH   = 0x80;                                                           //设置IIC为1:2占空比快速模式
  I2C->CCRL   = 0x07;                                                           //设置IIC的波特率,381KHZ传输速率
  I2C->TRISER = 0x09;                                                           //设置最大上升时间为1微秒
  I2C->FREQR  = 0x08;                                                           //设置IIC为8MHZ时钟频率

  I2C->OARL   = (0x56 << 0x01);                                                 //设置IIC自身的设备地址
  I2C->OARH   = 0x40;                                                           //设置IIC设备地址为7位模式
  I2C->CR1    = 0x01;                                                           //使能IIC设备
  I2C->CR2    = 0x04;                                                           //使能应答标识
}

/*
*函数名：MCU_IICReset( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：复位IIC通信接口
*/
void MCU_IICReset( void )
{
  I2C->CR2 |= 0x80;                                                             //复位一次IIC通信接口
  MCU_IICInit();                                                                //复位后再次初始化IIC通信接口
}

/*
*函数名：MCU_IICACK(unsigned char8 mode)
*入参：
  mode：应答信号使能
*返回值：无
*作用域：本地
*函数作用：设置IIC应答使能
*/
void MCU_IICACK(unsigned char8 mode)
{
  if(mode == 0x00){I2C->CR2 = 0x00;}else{I2C->CR2 = 0x04;}                      //设置应答使能
}

/*
*函数名：MCU_IICAddress(unsigned char8 address)
*入参：
  address：设备地址
*返回值：无
*作用域：本地
*函数作用：设置IIC通信接口的设备地址
*/
void MCU_IICAddress(unsigned char8 address)
{
  I2C->OARL = (address << 0x01);                                                //设置I2C通信接口自身的设备地址
}

/*
*函数名：MCU_IICStart(unsigned char8 address)
*入参：
  address：需要通信的从机地址
*返回值：起始条件发生是否超时
*作用域：本地
*函数作用：启动IIC产生起始位并发送从机地址
*/
unsigned short16 MCU_IICStart(unsigned char8 address)
{
  unsigned short16 timeout = 0x03FF;                                            //超时检测变量
  if(I2C->ITR != 0x00){return 0x00;}                                            //如果打开了中断则不允许设置为主模式

  I2C->CR2 |= 0x01;                                                             //产生起始条件
  while((I2C->SR1 & 0x01) == 0x00 && (-- timeout) > 0x00);                      //等待起始条件发送完成

  I2C->DR = address;                                                            //发送从设备地址
  while((I2C->SR1 & 0x02) == 0x00 && (-- timeout) > 0x00);                      //等待从设备应答

  address = I2C->SR3;                                                           //读一次SR3寄存器
  if(timeout == 0x00){MCU_IICReset();}                                          //如果发起通信超时则复位IIC通信接口
  return timeout;
}

/*
*函数名：MCU_IICStop( void )
*入参：无
*返回值：停止条件发生是否超时
*作用域：本地
*函数作用：使能IIC产生停止位
*/
unsigned short16 MCU_IICStop( void )
{
  unsigned short16 timeout = 0x03FF;                                            //超时检测变量
  if(I2C->ITR != 0x00){return 0x00;}                                            //如果打开了中断则不允许设置为主模式

  I2C->SR1 = 0x00;                                                              //清空传输标识
  I2C->CR2 |= 0x02;                                                             //产生停止条件
  while((I2C->CR2 & 0x02) > 0x00 && ((-- timeout) > 0x00));                     //等待停止条件发送完成

  if(timeout == 0x00){MCU_IICReset();}                                          //如果停止条件发送超时则复位IIC通信接口
  return timeout;
}

/*
*函数名：MCU_IICSend(unsigned char8 data)
*入参：
  data：需要发送的数据
*返回值：是否应答或超时
*作用域：本地
*函数作用：通过IIC发送一个字节
*/
unsigned char8 MCU_IICSend(unsigned char8 data)
{
  unsigned char8 timeout = 0xFF;                                                //超时检测变量
  if((I2C->SR3 & 0x04) == 0x00){return 0x00;}                                   //当前处在接收模式
  while((I2C->SR1 & 0x80) == 0x00 && ((-- timeout) > 0x00));                    //等待发送缓冲区空

  if(timeout == 0x00){                                                          //如果数据发送超时则复位IIC通信接口
    MCU_IICReset();                                                             //IIC通信接口复位
    return 0x00;                                                                //返回发送失败标志
  }else{
    I2C->SR1 = 0x00;                                                            //清空传输标志
    I2C->SR2 = 0x00;                                                            //清除状态标识
    I2C->DR  = data;                                                            //将数据写入发送缓存
    return ((I2C->SR2 & 0x04) == 0x00);                                         //检查从机应答
  }
}

/*
*函数名：MCU_IICRead( void )
*入参：无
*返回值：收到的数据及是否超时
*作用域：本地
*函数作用：读取IIC接收到的数据
*/
unsigned short16 MCU_IICRead( void )
{
  unsigned short16 regist = 0x00FF;                                             //超时检测变量
  if((I2C->SR3 & 0x04) > 0x00){return 0x00;}                                    //当前处在发送模式
  while((I2C->SR1 & 0x40) == 0x00 && ((-- regist) > 0x00));                     //等待当前的数据接收完成
  regist = ((regist << 0x08) | I2C->DR);                                        //读取收到的数据
  return regist;
}

/*
*函数名：IIC_Interrupt( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：IIC通信接口事件中断
*/
#pragma vector=0x15
__interrupt void IIC_Interrupt( void )
{
  unsigned char8 regist = I2C->SR1;                                             //读取SR1寄存器

  if((regist & 0x02) > 0x00){                                                   //地址匹配
  }

  if((regist & 0x04) > 0x00){                                                   //数据丢失
    MCU_IICReset();                                                             //复位IIC通信接口
  }

  if((regist & 0x10) > 0x00){                                                   //如果检测到停止位
    I2C->CR2 = 0x04;                                                            //重置CR2寄存器以清除停止位检测标志
  }

  if((regist & 0x40) > 0x00){                                                   //数据接收完成
    regist = MCU_IICRead();                                                     //读取收到的数据
  }

  if((regist & 0x80) > 0x00){                                                   //数据发送完成
  }
}

/*
*函数名：MCU_UART3Init(unsigned int32 baudrate)
*入参：
  baudrate：串口波特率
*返回值：无
*作用域：本地
*函数作用：初始化UART3通信接口
*/
void MCU_UART3Init(unsigned int32 baudrate)
{
  unsigned short16 regist = (((CLK->CKDIVR) >> 0x03) & 0x03);                   //获取主时钟的分频系数

  switch (regist){                                                              //计算当前的分频系数
    case (0x00):{regist = 0x01;break;}                                          //不分频
    case (0x01):{regist = 0x02;break;}                                          //二分频
    case (0x02):{regist = 0x04;break;}                                          //四分频
    case (0x03):{regist = 0x08;break;}                                          //八分频
    default:{regist = 0x08;break;}                                              //未定义状态统一默认为八分频
  }
  regist = 16000000 / (baudrate * regist);                                      //计算实际的波特率

  UART3->BRR2 = (((regist >> 0x08) & 0xF0) | (regist & 0x0F));                  //先设置波特率控制寄存器BRR2
  UART3->BRR1 = ((regist >> 0x04) & 0xFF);                                      //后设置波特率控制寄存器BRR1

  UART3->SR   = 0x00;                                                           //清空通信标志位
  UART3->CR2  = 0x2C;                                                           //开启发送和接收,并打开接收中断
  UART3->CR3  = 0x00;                                                           //设置为1个停止位,不启用同步时钟
  UART3->CR4  = 0x00;                                                           //不启用LIN功能
  UART3->CR6  = 0x00;                                                           //不启用LIN功能
  UART3->CR1  = 0x00;                                                           //使能UART并禁用校验功能
}

/*
*函数名：MCU_UART3Send(unsigned char8 data)
*入参：
  data：需要发送的数据
*返回值：是否等待超时
*作用域：本地
*函数作用：通过UART3发送一个数据
*/
unsigned char8 MCU_UART3Send(unsigned char8 data)
{
  unsigned char8 regist = 0xFF;                                                 //等待超时变量
  while(((UART3->SR & 0x80) == 0x00) && (-- regist) > 0x00);                    //等待发送缓存空
  UART3->DR = data;                                                             //将数据写入发送缓存
  return regist;
}

/*
*函数名：MCU_UART3Read( void )
*入参：无
*返回值：读取到的数据及是否超时
*作用域：本地
*函数作用：读取UART3收到的数据
*/
unsigned short16 MCU_UART3Read( void )
{
  unsigned short16 regist = 0xFF;                                               //等待超时变量
  while(((UART3->SR & 0x20) == 0x00) && (-- regist) > 0x00);                    //等待数据接收完成
  regist = ((regist << 0x08) | UART3->DR);                                      //读出收到的数据
  return regist;
}

/*
*函数名：UART3_TxInterrupt( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：UART3数据发送完成中断
*/
#pragma vector=0x16
__interrupt void UART3_TxInterrupt( void )
{
}

/*
*函数名：UART3_RxInterrupt( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：UART3数据接收完成中断
*/
#pragma vector=0x17
__interrupt void UART3_RxInterrupt( void )
{
  MCU_UART3Read();                                                              //读取接收到的数据
}

/*
*函数名：MCU_ADCInit( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化模数转换器
*/
void MCU_ADCInit( void )
{
//  ADC2->CSR  = 0x20;                                                            //清除转换标志及转换通道并打开转换完成中断
  ADC2->CR2  = 0x08;                                                            //设置不外部触发、数据右对齐、不开启扫描
  ADC2->TDRH = 0xFF;                                                            //关闭所有ADC端口引脚的施密特触发器
  ADC2->TDRL = 0xFF;                                                            //关闭所有ADC端口引脚的施密特触发器
  ADC2->CR1  = 0x01;                                                            //设置ADC不分频、单次转换
}

/*
*函数名：MCU_ADCChannel(unsigned char8 channel)
*入参：
  channl：模拟通道号通道
*返回值：无
*作用域：本地
*函数作用：更改模数转换器的采样通道
*/
void MCU_ADCChannel(unsigned char8 channel)
{
  ADC2->CSR = (channel & 0x0F);                                                 //写入转换通道
}

/*
*函数名：MCU_ADCStart( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：启动一次模数转换
*/
void MCU_ADCStart( void )
{
  ADC2->CR1 |= 0x01;                                                            //启动模数转换器开始转换
}

/*
*函数名：MCU_GetADCValue( void )
*入参：无
*返回值：转换好的数字量
*作用域：本地
*函数作用：读取模数转换器的转换值
*/
unsigned short16 MCU_ADCValue( void )
{
  unsigned short16 regist;
  while((ADC2->CSR & 0x80) == 0x00);                                            //等待转换完成
  ADC2->CSR &= 0x7F;                                                            //清空转换标志位
  regist = ADC2->DRL;                                                           //获取模数转换器的低位
  regist = ((ADC2->DRH << 0x08) | regist);                                      //获取模数转换器的高位
  return regist;
}

/*
*函数名：ADC_Interrupt( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：数模转换完成中断
*/
#pragma vector=0x18
__interrupt void ADC_Interrupt( void )
{
  MCU_ADCValue();                                                               //获取ADC的转换数据
}

/*
*函数名：MCU_TIM4Init( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化TIM4定时器
*/
void MCU_TIM4Init( void )
{
  TIM4->SR1  = 0x00;                                                            //清空事件标识位
  TIM4->PSCR = 0x06;                                                            //设置分频器
  TIM4->CNTR = 0x00;                                                            //清空计时器
  TIM4->ARR  = 0xFA;                                                            //设置预装载寄存器
  TIM4->IER  = 0x01;                                                            //中断控制寄存器,允许更新中断
  TIM4->CR1  = 0x81;                                                            //开启预装载并使能定时器
}

/*
*函数名：TIM4_Interrupt(void)
*入参：无
*返回值：无
*作用域：本地
*函数作用：TIM4的更新中断处理函数
*/
#pragma vector=0x19
__interrupt void TIM4_Interrupt(void)
{
/*************由于编译器没有将R4到R7寄存器入栈需要在中断进入时压栈*************/
  asm("         LDW     X, 0x08         ");                                     //R4转移到X寄存器
  asm("         PUSHW   X               ");                                     //X寄存器入栈
  asm("         LDW     X, 0x0A         ");                                     //R5转移到X寄存器
  asm("         PUSHW   X               ");                                     //X寄存器入栈
  asm("         LDW     X, 0x0C         ");                                     //R6转移到X寄存器
  asm("         PUSHW   X               ");                                     //X寄存器入栈
  asm("         LDW     X, 0x0E         ");                                     //R7转移到X寄存器
  asm("         PUSHW   X               ");                                     //X寄存器入栈

  TIM4->SR1 = 0x00;
  (Myos_Core0.ThreadDispatch)();                                                //线程调度执行

/*************由于编译器没有将R4到R7寄存器出栈需要在中断出栈时出栈*************/
  asm("         POPW    X               ");                                     //出栈到X寄存器
  asm("         LDW     0x0E, X         ");                                     //转移到R7
  asm("         POPW    X               ");                                     //出栈到X寄存器
  asm("         LDW     0x0C, X         ");                                     //转移到R6
  asm("         POPW    X               ");                                     //出栈到X寄存器
  asm("         LDW     0x0A, X         ");                                     //转移到R5
  asm("         POPW    X               ");                                     //出栈到X寄存器
  asm("         LDW     0x08, X         ");                                     //转移到R4
}

/*
*函数名：MCU_ChipInit( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化芯片的外设组件
*/
void MCU_ChipInit( void )
{
  MCU_FrequencyInit();                                                          //系统主时钟初始化
  MCU_ExitInterruptInit();                                                      //初始化外部中断触发方式
  MCU_GPIOAInit();                                                              //初始化GPIOA端口
  MCU_GPIOBInit();                                                              //初始化GPIOB端口
  MCU_GPIOCInit();                                                              //初始化GPIOC端口
  MCU_GPIODInit();                                                              //初始化GPIOD端口
  MCU_GPIOEInit();                                                              //初始化GPIOE端口
  MCU_GPIOFInit();                                                              //初始化GPIOF端口
  MCU_GPIOGInit();                                                              //初始化GPIOG端口
//  MCU_FlashInit();                                                              //初始化设备存储器
//  MCU_BEEPInit();                                                               //初始化蜂鸣器
  MCU_SPIInit();                                                                //初始化SPI通信接口
//  MCU_TIM1Init();                                                               //初始化TIM1定时器
//  MCU_TIM2Init();                                                               //初始化TIM2定时器
  MCU_TIM3Init();                                                               //初始化TIM3定时器
//  MCU_UART1Init(115200);                                                        //初始化UART1通信接口
//  MCU_IICInit();                                                                //初始化IIC通信接口
  MCU_UART3Init(115200);                                                        //初始化UART3通信接口
//  MCU_ADCInit();                                                                //初始化模数转换器
  MCU_TIM4Init();                                                               //初始化TIM4定时器

  OS_Delay(0x8000);                                                             //等待初始化完成
  OS_Delay(0x8000);                                                             //等待初始化完成
}
