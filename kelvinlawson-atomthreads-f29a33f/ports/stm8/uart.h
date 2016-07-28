#include "atom.h"

#define RXBUFFLEN 50

typedef struct rxbuff{
  uint8_t buff[RXBUFFLEN];
  uint8_t len;                    //使用长度
}rxDataBuffStruct;

extern rxDataBuffStruct rxDataBuff;

/*
 * Perform UART startup initialization.
 */
extern int	uart_init(uint32_t baudrate);
