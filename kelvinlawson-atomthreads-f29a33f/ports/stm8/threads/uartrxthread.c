#include "stdio.h"

#include "stm8s.h"
#include "uart.h"

#include "atom.h"
#include "atomtimer.h"
#include "atomsem.h"
#include "atomqueue.h"
#include "atommutex.h"

#include "uartrxthread.h"
#include "displaythread.h"

extern ATOM_TCB uartProcess_tcb;
extern ATOM_SEM uartRxsem;
extern ATOM_SEM disCommondsem;
extern ATOM_MUTEX disCommondmutex;

extern disComdata disCommandData;
extern uint8_t rxDataBuff[50];
void uartProcess_thread_func (uint32_t param);

void uartProcess_thread_func (uint32_t param)
{
  volatile uint32_t use=0,free=0;
  if (uart_init(115200) != 0)
  {
      /* Error initialising UART */
  }
  printf("uart test\r\n");
  while (1)
  {
    //wait forever
    if (atomSemGet(&uartRxsem, 0)==ATOM_OK)
    {
      if (atomMutexGet(&disCommondmutex, 0) == ATOM_OK)
      {
        for (uint8_t i=0;i<50;i++)
        {
          disCommandData.buff[i] = rxDataBuff[i];
        }
        disCommandData.commandlist |= NEWCOMMAND;
        atomMutexPut(&disCommondmutex);
      }
      atomSemPut (&disCommondsem);
    }
    
    atomThreadStackCheck (&uartProcess_tcb, (uint32_t*)&use, (uint32_t*)&free);
    if (free<100)
      printf("display stack too low");
  }
}