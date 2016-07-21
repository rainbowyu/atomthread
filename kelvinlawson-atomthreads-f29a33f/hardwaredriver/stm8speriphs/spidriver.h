#ifndef _SPIDRIVER_H_
#define _SPIDRIVER_H_

#define SPI_SCK_PIN  GPIOC,GPIO_PIN_5
#define SPI_MOSI_PIN GPIOC,GPIO_PIN_6
#define SPI_MISO_PIN GPIOC,GPIO_PIN_7
extern u8 spiSendByte( u8 byteValue );
#endif