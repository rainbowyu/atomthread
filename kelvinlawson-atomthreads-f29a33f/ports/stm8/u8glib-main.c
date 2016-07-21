#include "stdio.h"  
#include "atom.h"
#include "atomport-private.h"
#include "atomport-tests.h"
#include "atomtests.h"
#include "atomtimer.h"
#include "uart.h"
#include "stm8s.h"

#include "u8g.h"
#include "u8g_stm8s.h"
/* Constants */

/*
 * Idle thread stack size
 *
 * This needs to be large enough to handle any interrupt handlers
 * and callbacks called by interrupt handlers (e.g. user-created
 * timer callbacks) as well as the saving of all context when
 * switching away from this thread.
 *
 * In this case, the idle stack is allocated on the BSS via the
 * idle_thread_stack[] byte array.
 */
#define IDLE_STACK_SIZE_BYTES       128


/*
 * Main thread stack size
 *
 * Note that this is not a required OS kernel thread - you will replace
 * this with your own application thread.
 *
 * In this case the Main thread is responsible for calling out to the
 * test routines. Once a test routine has finished, the test status is
 * printed out on the UART and the thread remains running in a loop
 * flashing a LED.
 *
 * The Main thread stack generally needs to be larger than the idle
 * thread stack, as not only does it need to store interrupt handler
 * stack saves and context switch saves, but the application main thread
 * will generally be carrying out more nested function calls and require
 * stack for application code local variables etc.
 *
 * With all OS tests implemented to date on the STM8, the Main thread
 * stack has not exceeded 256 bytes. To allow all tests to run we set
 * a minimum main thread stack size of 204 bytes. This may increase in
 * future as the codebase changes but for the time being is enough to
 * cope with all of the automated tests.
 */
#define DISPLAY_STACK_SIZE_BYTES           1024
#define UARTPROCESS_STACK_SIZE_BYTES       500

/*
 * Startup code stack
 *
 * Some stack space is required at initial startup for running the main()
 * routine. This stack space is only temporarily required at first bootup
 * and is no longer required as soon as the OS is started. By default
 * Cosmic sets this to the top of RAM and it grows down from there.
 *
 * Because we only need this temporarily you may reuse the area once the
 * OS is started, and are free to use some area other than the top of RAM.
 * For convenience we just use the default region here.
 */


/* Local data */

/* Application threads' TCBs */
static ATOM_TCB display_tcb;
static ATOM_TCB uartProcess_tcb;

/* Main thread's stack area (large so place outside of the small page0 area on STM8) */
NEAR static uint8_t display_thread_stack[DISPLAY_STACK_SIZE_BYTES];
NEAR static uint8_t uartProcess_thread_stack[UARTPROCESS_STACK_SIZE_BYTES];

/* Idle thread's stack area (large so place outside of the small page0 area on STM8) */
NEAR static uint8_t idle_thread_stack[IDLE_STACK_SIZE_BYTES];

/* Forward declarations */
static void display_thread_func (uint32_t param);
static void uartProcess_thread_func (uint32_t param);

/**
 * \b main
 *
 * Program entry point.
 *
 * Sets up the STM8 hardware resources (system tick timer interrupt) necessary
 * for the OS to be started. Creates an application thread and starts the OS.
 *
 * If the compiler supports it, stack space can be saved by preventing
 * the function from saving registers on entry. This is because we
 * are called directly by the C startup assembler, and know that we will
 * never return from here. The NO_REG_SAVE macro is used to denote such 
 * functions in a compiler-agnostic way, though not all compilers support it.
 *
 */
NO_REG_SAVE void main ( void )
{
    int8_t status;

    /**
     * Note: to protect OS structures and data during initialisation,
     * interrupts must remain disabled until the first thread
     * has been restored. They are reenabled at the very end of
     * the first thread restore, at which point it is safe for a
     * reschedule to take place.
     */

    /* Initialise the OS before creating our threads */
    status = atomOSInit(&idle_thread_stack[0], IDLE_STACK_SIZE_BYTES, TRUE);
    if (status == ATOM_OK)
    {
        /* Enable the system tick timer */
        archInitSystemTickTimer();

        /* Create an application thread */
        status = atomThreadCreate(&display_tcb,
                     10, display_thread_func, 0,
                     &display_thread_stack[0],
                     DISPLAY_STACK_SIZE_BYTES,
                     TRUE);
        
        if (status == ATOM_OK)
        {
            /**
             * First application thread successfully created. It is
             * now possible to start the OS. Execution will not return
             * from atomOSStart(), which will restore the context of
             * our application thread and start executing it.
             *
             * Note that interrupts are still disabled at this point.
             * They will be enabled as we restore and execute our first
             * thread in archFirstThreadRestore().
             */
            atomOSStart();
        }
    }

    /* There was an error starting the OS if we reach here */
    while (1)
    {
    }

}

static void uartProcess_thread_func (uint32_t param)
{
  if (uart_init(115200) != 0)
  {
      /* Error initialising UART */
  }
  printf("uart test\r\n");
  while (1)
  {
    printf("uart test\r\n");
    atomTimerDelay (SYSTEM_TICKS_PER_SEC);
  }
}
static void display_thread_func (uint32_t param)
{
  u8g_t u8g;
  int8_t status;
  status = atomThreadCreate(&uartProcess_tcb,
             10, uartProcess_thread_func, 0,
             &uartProcess_thread_stack[0],
             UARTPROCESS_STACK_SIZE_BYTES,
             TRUE);
  
  u8g_InitComFn(&u8g,&u8g_dev_uc1701_mini12864_hw_spi,u8g_com_hw_spi_fn);

  while (1)
  {
     u8g_SetFont(&u8g, u8g_font_osb18);
     u8g_FirstPage(&u8g);  
     do  
     {    
          u8g_DrawHLine(&u8g, 0, 20, 128);
          u8g_DrawStr(&u8g, 0, 48, "Hello");
     } while ( u8g_NextPage(&u8g) ); 
  }
}

