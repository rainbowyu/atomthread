#include "stdio.h"
#include "atom.h"
#include "atomport-private.h"
#include "atomtests.h"
#include "atomtimer.h"
#include "atomsem.h"
#include "atomqueue.h"
#include "atommutex.h"

#include "uart.h"
#include "stm8s.h"
#include "stm8s_clk.h"

#include "mainthread.h"
#include "uartrxthread.h"
#ifdef GPS
  #include "gpsprocessthread.h"
#endif
//#include "cmdshellthread.h"
//#include "cmdshell.h"
//#include "filethread.h"
//#include "displaythread.h"


taskStateStruct taskState;
/* Main thread's stack area (large so place outside of the small page0 area on STM8) */
//NEAR static uint8_t display_thread_stack[DISPLAY_STACK_SIZE_BYTES];
//NEAR static uint8_t file_thread_stack[FILE_STACK_SIZE_BYTES];

NEAR static uint8_t uartProcess_thread_stack[UARTPROCESS_STACK_SIZE_BYTES];
NEAR static uint8_t gpsProcess_thread_stack[GPSPROCESS_STACK_SIZE_BYTES];
//NEAR static uint8_t cmdshell_thread_stack[CMDSHELL_STACK_SIZE_BYTES];

/* Idle thread's stack area (large so place outside of the small page0 area on STM8) */
NEAR static uint8_t idle_thread_stack[IDLE_STACK_SIZE_BYTES];

NO_REG_SAVE void main ( void )
{
    int8_t status;
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

    if (uart_init(115200) != 0)
    {
    /* Error initialising UART */
    }
    
    /* Initialise the OS before creating our threads */
    status = atomOSInit(&idle_thread_stack[0], IDLE_STACK_SIZE_BYTES, TRUE);
    
    if (status == ATOM_OK)
    {
        /* Enable the system tick timer */
        archInitSystemTickTimer();

        /* Create an application thread */
//        status += atomThreadCreate(&display_tcb,
//                     20, display_thread_func, 0,
//                     &display_thread_stack[0],
//                     DISPLAY_STACK_SIZE_BYTES,
//                     TRUE);

        status += atomThreadCreate(&uartProcess_tcb,
                     10, uartProcess_thread_func, 0,
                     &uartProcess_thread_stack[0],
                     UARTPROCESS_STACK_SIZE_BYTES,
                     TRUE,"UART_PRC");
#ifdef GPS        
        status += atomThreadCreate(&gpsProcess_tcb,
                     9, gpsProcess_thread_func, 0,
                     &gpsProcess_thread_stack[0],
                     GPSPROCESS_STACK_SIZE_BYTES,
                     TRUE,"GPS_PRC");
#endif
//        status += atomThreadCreate(&cmdshell_tcb,
//                     10, cmdshell_thread_func, 0,
//                     &cmdshell_thread_stack[0],
//                     CMDSHELL_STACK_SIZE_BYTES,
//                     TRUE,"CMD_SHELL");

//        status += atomThreadCreate(&file_tcb,
//                     9, file_thread_func, 0,
//                     &file_thread_stack[0],
//                     FILE_STACK_SIZE_BYTES,
//                     TRUE,"FILE_SYS");

        status += atomSemCreate (&uart3Rxsem, 0);
#ifdef GPS
        status += atomSemCreate (&gpsDatasem, 0);
        status += atomMutexCreate (&gpsDatamutex);
#endif
//        if (atomSemCreate (&cmdShellsem, 0) != ATOM_OK)
//        {
//
//        }
//        if ( atomSemCreate (&fileCommondsem, 0) != ATOM_OK)
//        {
//  
//        }

        if (status == ATOM_OK)
        {
            atomOSStart();
        }
    }

    while (1)
    {
    }
}
