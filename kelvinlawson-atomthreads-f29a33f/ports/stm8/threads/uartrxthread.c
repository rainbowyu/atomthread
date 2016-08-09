#include "stdio.h"

#include "stm8s.h"
#include "uart.h"

#include "atom.h"
#include "atomtimer.h"
#include "atomsem.h"
#include "atommutex.h"

#include "uartrxthread.h"

#ifdef CLI
  #include "cmdshellthread.h"
#endif

#ifdef GPS
  #include "gpsprocessthread.h"
#endif

#include "mainthread.h"
//#include "displaythread.h"

ATOM_TCB uartProcess_tcb;
ATOM_SEM uart3Rxsem;

void uartProcess_thread_func (uint32_t param);

void uartProcess_thread_func (uint32_t param)
{
  volatile uint32_t use=0,free=0;
  while (1)
  {
    //wait forever
    if (atomSemGet(&uart3Rxsem, 0) == ATOM_OK)
    {
      #ifdef GPS
        atomMutexGet (&gpsDatamutex,0);
        for (uint8_t i=0;i<rxDataBuff.len;i++)
        {
          GPSData.buff[i] = rxDataBuff.buff[i];
        }
        atomMutexPut (&gpsDatamutex);
        atomSemPut (&gpsDatasem);
      #endif      
      
      #ifdef CLI
        //UART send data to cli
        for (uint8_t i=0;i<rxDataBuff.len;i++)
        {
          cmdData.buff[i] = rxDataBuff.buff[i];
        }
        cmdData.bufflen = rxDataBuff.len;
        atomSemPut (&cmdShellsem);
      #endif
      
      //get the used RAM
      atomThreadStackCheck (&uartProcess_tcb, (uint32_t*)&use, (uint32_t*)&free);
      taskState.taskRAMMax[uartProcess_tcb.threadNum][0]=(uint16_t)use;
      taskState.taskRAMMax[uartProcess_tcb.threadNum][1]=(uint16_t)free;
    }
  }
}
