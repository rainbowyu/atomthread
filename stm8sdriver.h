#ifndef _STM8SDRIVER_H_
#define _STM8SDRIVER_H_
#include "myosconfigure.h"



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
void MCU_OptionSet(unsigned char8 option, unsigned char8 noption, unsigned char8 value);

/*
*函数名：MCU_ChipSerial(unsigned char8 *cache)
*入参：
  cache：芯片序列号的缓存地址
*返回值：无
*作用域：本地
*函数作用：获取芯片的唯一序列号
*/
void MCU_ChipSerial(unsigned char8 *cache);

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
void MCU_EEPROMWrite(unsigned short16 offset, unsigned short16 length, unsigned char8 *cache);

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
void MCU_EEPROMRead(unsigned short16 offset, unsigned short16 length, unsigned char8 *cache);

/*
*函数名：MCU_BEEPSet(unsigned char8 status)
*入参：
  status：状态变量
*返回值：无
*作用域：本地
*函数作用：设置蜂鸣器的状态
*/
void MCU_BEEPSet(unsigned char8 status);

/*
*函数名：MCU_SPIRead(unsigned char8 data)
*入参：
  data：需要回复的数据
*返回值：主机发送的数据及是否超时
*作用域：本地
*函数作用：读取SPI接收到的数据并在下一个通信周期回复主机数据
*/
unsigned short16 MCU_SPIRead(unsigned char8 data);

/*
*函数名：MCU_SPISend(unsigned char8 data)
*入参：
  data：需要发送的数据
*返回值：从机回复的数据及是否超时
*作用域：本地
*函数作用：通过SPI发送数据并接受从机返回的数据
*/
unsigned short16 MCU_SPISend(unsigned char8 data);

/*
*函数名：MCU_UART1Send(unsigned char8 data)
*入参：
  data：需要发送的数据
*返回值：是否等待超时
*作用域：本地
*函数作用：通过UART1发送一个数据
*/
unsigned char8 MCU_UART1Send(unsigned char8 data);

/*
*函数名：MCU_UART1Read( void )
*入参：无
*返回值：读取到的数据及是否超时
*作用域：本地
*函数作用：读取UART1收到的数据
*/
unsigned short16 MCU_UART1Read( void );

/*
*函数名：MCU_IICReset( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：复位IIC通信接口
*/
void MCU_IICReset( void );

/*
*函数名：MCU_IICACK(unsigned char8 mode)
*入参：
  mode：应答信号使能
*返回值：无
*作用域：本地
*函数作用：设置IIC应答使能
*/
void MCU_IICACK(unsigned char8 mode);

/*
*函数名：MCU_IICAddress(unsigned char8 address)
*入参：
  address：设备地址
*返回值：无
*作用域：本地
*函数作用：设置IIC通信接口的设备地址
*/
void MCU_IICAddress(unsigned char8 address);

/*
*函数名：MCU_IICStart(unsigned char8 address)
*入参：
  address：需要通信的从机地址
*返回值：起始条件发生是否超时
*作用域：本地
*函数作用：启动IIC产生起始位并发送从机地址
*/
unsigned short16 MCU_IICStart(unsigned char8 address);

/*
*函数名：MCU_IICStop( void )
*入参：无
*返回值：停止条件发生是否超时
*作用域：本地
*函数作用：使能IIC产生停止位
*/
unsigned short16 MCU_IICStop( void );

/*
*函数名：MCU_IICSend(unsigned char8 data)
*入参：
  data：需要发送的数据
*返回值：是否应答或超时
*作用域：本地
*函数作用：通过IIC发送一个字节
*/
unsigned char8 MCU_IICSend(unsigned char8 data);

/*
*函数名：MCU_IICRead( void )
*入参：无
*返回值：收到的数据及是否超时
*作用域：本地
*函数作用：读取IIC接收到的数据
*/
unsigned short16 MCU_IICRead( void );

/*
*函数名：MCU_UART3Send(unsigned char8 data)
*入参：
  data：需要发送的数据
*返回值：是否等待超时
*作用域：本地
*函数作用：通过UART3发送一个数据
*/
unsigned char8 MCU_UART3Send(unsigned char8 data);

/*
*函数名：MCU_UART3Read( void )
*入参：无
*返回值：读取到的数据及是否超时
*作用域：本地
*函数作用：读取UART3收到的数据
*/
unsigned short16 MCU_UART3Read( void );

/*
*函数名：MCU_ADCChannel(unsigned char8 channel)
*入参：
  channl：模拟通道号通道
*返回值：无
*作用域：本地
*函数作用：更改模数转换器的采样通道
*/
void MCU_ADCChannel(unsigned char8 channel);

/*
*函数名：MCU_ADCStart( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：启动一次模数转换
*/
void MCU_ADCStart( void );

/*
*函数名：MCU_GetADCValue( void )
*入参：无
*返回值：转换好的数字量
*作用域：本地
*函数作用：读取模数转换器的转换值
*/
unsigned short16 MCU_ADCValue( void );

/*
*函数名：MCU_ChipInit( void )
*入参：无
*返回值：无
*作用域：本地
*函数作用：初始化芯片的外设组件
*/
void MCU_ChipInit( void );



#endif

