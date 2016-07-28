#include "stdio.h"

#include "stm8s.h"
#include "uart.h"

#include "atom.h"
#include "atomtimer.h"
#include "atomsem.h"
#include "atomqueue.h"
#include "atommutex.h"

#include "uartrxthread.h"
#include "cmdshellthread.h"
#include "mainthread.h"
//#include "displaythread.h"

ATOM_TCB uartProcess_tcb;
ATOM_SEM uartRxsem;

void uartProcess_thread_func (uint32_t param);

/*
typedef struct Commanddatas{
  uint8_t buff[CMDBUFFLEN];
  uint8_t cmdParam[ARGNUM][ARGLEN];
  uint8_t cmd[CMDLEN];
  uint8_t bufflen;
}Commanddata;
*/

void uartProcess_thread_func (uint32_t param)
{
  volatile uint32_t use=0,free=0;
  if (uart_init(115200) != 0)
  {
    /* Error initialising UART */
  }
  while (1)
  {
    //wait forever
    if (atomSemGet(&uartRxsem, 0) == ATOM_OK)
    {
      //UART send data to cli
      for (uint8_t i=0;i<rxDataBuff.len;i++)
      {
        cmdData.buff[i] = rxDataBuff.buff[i];
      }
      cmdData.bufflen = rxDataBuff.len;
      atomSemPut (&cmdShellsem);
      
      //get the used RAM
      atomThreadStackCheck (&uartProcess_tcb, (uint32_t*)&use, (uint32_t*)&free);
      taskState.taskRAMMax[uartProcess_tcb.threadNum][0]=(uint16_t)use;
      taskState.taskRAMMax[uartProcess_tcb.threadNum][1]=(uint16_t)free;
    }
  }
}
