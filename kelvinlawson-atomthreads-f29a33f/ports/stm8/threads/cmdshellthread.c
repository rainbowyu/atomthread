#include "stdio.h"
#include "string.h"

#include "stm8s.h"
//#include "u8g.h"
//#include "u8g_stm8s.h"

#include "atom.h"
#include "atomtimer.h"
#include "atomsem.h"
#include "atomqueue.h"
#include "atommutex.h"

#include "cmdshellthread.h"
#include "cmdShell.h"
#include "mainthread.h"

Commanddata cmdData = {0};

ATOM_TCB cmdshell_tcb;
ATOM_SEM cmdShellsem;

void cmdshell_thread_func (uint32_t param)
{
  uint8_t i=0;
  volatile uint32_t use=0,free=0;
  int8_t recArgNum=0;
  while (1)
  {

    if (atomSemGet(&cmdShellsem, 0) == ATOM_OK)
    {
      //get the cmd and param 
      recArgNum=cmdParser(&cmdData);
      for(i=0;i<SHELLCMDNUM;i++)                                                
      {
        if(!strcmp((const char *)CMDList[i].cmd_name,(const char *)cmdData.cmd))  //get the cmd
        {  
          if(recArgNum<0 || recArgNum> CMDList[i].max_args)
          {
            printf("param num error!\n");
          }
          else
          {
            CMDList[i].handle(recArgNum,(void *)&cmdData);
          }
          break;
        }
      }
      if(i>=SHELLCMDNUM)
      {
          printf("cant find the cmd!\n");
      }
      
      //get the used RAM
      atomThreadStackCheck (&cmdshell_tcb, (uint32_t*)&use, (uint32_t*)&free);
      taskState.taskRAMMax[cmdshell_tcb.threadNum][0]=(uint16_t)use;
      taskState.taskRAMMax[cmdshell_tcb.threadNum][1]=(uint16_t)free;
    }
  }
}
