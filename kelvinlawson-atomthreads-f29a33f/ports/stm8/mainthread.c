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
//#include "displaythread.h"
#include "uartrxthread.h"
#include "filethread.h"

/* Main thread's stack area (large so place outside of the small page0 area on STM8) */
//NEAR static uint8_t display_thread_stack[DISPLAY_STACK_SIZE_BYTES];
NEAR static uint8_t uartProcess_thread_stack[UARTPROCESS_STACK_SIZE_BYTES];
NEAR static uint8_t file_thread_stack[FILE_STACK_SIZE_BYTES];

/* Idle thread's stack area (large so place outside of the small page0 area on STM8) */
NEAR static uint8_t idle_thread_stack[IDLE_STACK_SIZE_BYTES];

NO_REG_SAVE void main ( void )
{
    int8_t status;
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    
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
                     TRUE);
        
        status += atomThreadCreate(&file_tcb,
                     20, file_thread_func, 0,
                     &file_thread_stack[0],
                     FILE_STACK_SIZE_BYTES,
                     TRUE);
        
        if (atomSemCreate (&uartRxsem, 0) != ATOM_OK)
        {
          //printf ("Error creating uartRx semaphore \n");
        }
//        if ( atomSemCreate (&disCommondsem, 0) != ATOM_OK)
//        {
//          //printf ("Error creating disCommond semaphore \n");
//        }
        if ( atomSemCreate (&fileCommondsem, 0) != ATOM_OK)
        {
          //printf ("Error creating disCommond semaphore \n");
        }
        
        //所有任务创建成功
        if (status == ATOM_OK)
        {
            atomOSStart();
        }
    }

    /* 代码不会运行到此处 */
    while (1)
    {
    }
}




