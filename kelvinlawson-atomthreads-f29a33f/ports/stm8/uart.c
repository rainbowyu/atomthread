#include <stdio.h>

#include "stm8s.h"

#include "atom.h"
#include "atommutex.h"
#include "atomsem.h"
#include "uart.h"
#include "stm8s_uart3.h"
#include "stm8s_itc.h"
#include "uartrxthread.h"

rxDataBuffStruct rxDataBuff;
extern ATOM_SEM uartRxsem;
/*
 * Semaphore for single-threaded access to UART device
 */
static ATOM_MUTEX uart_mutex;


/*
 * Initialize the UART to requested baudrate, tx/rx, 8N1.
 */
int uart_init(uint32_t baudrate)
{
  int status;

  /**
   * Set up UART3 for putting out debug messages.
   * This the UART used on STM8S Discovery, change if required.
   */
  UART3_DeInit();
  UART3_Init (baudrate, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO,
              UART3_MODE_TXRX_ENABLE);
  UART3_ITConfig(UART3_IT_RXNE, ENABLE);
  UART3_Cmd(ENABLE);
  enableInterrupts();
  /* Create a mutex for single-threaded putchar() access */
  if (atomMutexCreate (&uart_mutex) != ATOM_OK)
  {
    status = -1;
  }
  else
  {
    status = 0;
  }

  /* Finished */
  return (status);
}


/**
 * \b uart_putchar
 *
 * Write a char out via UART3
 *
 * @param[in] c Character to send
 *
 * @return Character sent
 */
char uart_putchar (char c)
{
    /* Block on private access to the UART */
    if (atomMutexGet(&uart_mutex, 0) == ATOM_OK)
    {
        /* Convert \n to \r\n */
        if (c == '\n')
            putchar('\r');

        /* Write a character to the UART3 */
        UART3_SendData8(c);

        /* Loop until the end of transmission */
        while (UART3_GetFlagStatus(UART3_FLAG_TXE) == RESET)
            ;

        /* Return mutex access */
        atomMutexPut(&uart_mutex);

    }

    return (c);
}


/* COSMIC: Requires putchar() routine to override stdio */
#if defined(__CSMC__)
/**
 * \b putchar
 *
 * Retarget putchar() to use UART3
 *
 * @param[in] c Character to send
 *
 * @return Character sent
 */
char putchar (char c)
{
    return (uart_putchar(c));
}
#endif /* __CSMC__ */


/* RAISONANCE: Requires putchar() routine to override stdio */
#if defined(__RCSTM8__)
/**
 * \b putchar
 *
 * Retarget putchar() to use UART3
 *
 * @param[in] c Character to send
 *
 * @return 1 on success
 */
int putchar (char c)
{
    uart_putchar(c);
    return (1);
}
#endif /* __RCSTM8__ */


/* IAR: Requires __write() routine to override stdio */
#if defined(__IAR_SYSTEMS_ICC__)
/**
 * \b __write
 *
 * Override for IAR stream output
 *
 * @param[in] handle Stdio handle. -1 to flush.
 * @param[in] buf Pointer to buffer to be written
 * @param[in] bufSize Number of characters to be written
 *
 * @return Number of characters sent
 */
size_t __write(int handle, const unsigned char *buf, size_t bufSize)
{
    size_t chars_written = 0;

    /* Ignore flushes */
    if (handle == -1)
    {
      chars_written = (size_t)0;
    }
    /* Only allow stdout/stderr output */
    else if ((handle != 1) && (handle != 2))
    {
      chars_written = (size_t)-1;
    }
    /* Parameters OK, call the low-level character output routine */
    else
    {
        while (chars_written < bufSize)
        {
            uart_putchar (buf[chars_written]);
            chars_written++;
        }
    }

    return (chars_written);
}
#endif /* __IAR_SYSTEMS_ICC__ */

#pragma vector = ITC_IRQ_UART3_RX + 2
__interrupt void UART3_RX_IRQHandler(void)
{
  uint8_t temp;
  static uint8_t * p = rxDataBuff.buff;
  static uint8_t len = 0;
  if( UART3_GetITStatus(UART3_IT_RXNE) == SET && \
      UART3_GetFlagStatus(UART3_FLAG_RXNE) == SET)
  {
    UART3_ClearITPendingBit (UART3_IT_RXNE);
    atomIntEnter ();
    temp = (UART3_ReceiveData8());
    *p = temp;

    if (p<=&rxDataBuff.buff[RXBUFFLEN-1]){p++;len++;}
    else
    {
      printf ("RX buff overflow\n");
    }

    if (temp=='\n'){
      *(p+1)='\0';                              //数据最后添加 '\0'     
      rxDataBuff.len=len;
      
      len=0;                                //长度归零
      p=rxDataBuff.buff;                    //指针归位
      
      atomSemPut (&uart3Rxsem);
    }
    atomIntExit (0);
  }
}
