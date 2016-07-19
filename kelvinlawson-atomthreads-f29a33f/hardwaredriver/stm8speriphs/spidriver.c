#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_spi.h"
u8 spiSendByte( u8 byteValue );

u8 spiSendByte( u8 byteValue )
{
  u8 data_buf;
  while(SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET); 
  SPI_SendData(byteValue);
  while(SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);  
  data_buf = SPI_ReceiveData(); 
	return 	data_buf;
}