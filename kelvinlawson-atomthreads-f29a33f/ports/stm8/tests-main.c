#include "stdio.h"  
#include "atom.h"
#include "atomport-private.h"
#include "atomport-tests.h"
#include "atomtests.h"
#include "atomtimer.h"
#include "uart.h"
#include "stm8s.h"
#include "ff.h"
#include "diskio.h"
#include "spiflash.h"
#include "filecommon.h"
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
#define MAIN_STACK_SIZE_BYTES       1000


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
static ATOM_TCB main_tcb;

/* Main thread's stack area (large so place outside of the small page0 area on STM8) */
NEAR static uint8_t main_thread_stack[MAIN_STACK_SIZE_BYTES];

/* Idle thread's stack area (large so place outside of the small page0 area on STM8) */
NEAR static uint8_t idle_thread_stack[IDLE_STACK_SIZE_BYTES];


/* Forward declarations */
static void main_thread_func (uint32_t param);

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
        status = atomThreadCreate(&main_tcb,
                     TEST_THREAD_PRIO, main_thread_func, 0,
                     &main_thread_stack[0],
                     MAIN_STACK_SIZE_BYTES,
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

const BYTE writeByte[3]="OK";
static void main_thread_func (uint32_t param)
{
  FATFS fs;           /* File system object */
  FIL fil;            /* File object */
  FRESULT res;        /* API result code */
  UINT bw;            /* Bytes written */
  //BYTE work[_MAX_SS]={0}; /* Work area (larger is better for process time) */
  //BYTE readByte[20];
  //BYTE writeByte[3]="OK";
  /* Initialise UART (115200bps) */
  if (uart_init(115200) != 0)
  {
      /* Error initialising UART */
  }
  printf("uart test\r\n");
  /* Configure GPIO for flashing the STM8S Discovery LED on GPIO D0 */
  GPIO_DeInit(GPIOC);
  GPIO_Init(GPIOC, GPIO_PIN_1, GPIO_MODE_OUT_PP_LOW_FAST);
  
  disk_initialize (0);
  /* Register work area */
  res=f_mount(&fs, "", 1);
  if (res)
  {
    printf("¹ÒÔØÊ§°Ü,Ê§°Ü´úÂë:%u\r\n",res);
    atomTimerDelay (1);
  }
  /* Create a file as new */
  
  res = f_open(&fil, "hello.txt", FA_READ	| FA_WRITE);
  if (res)
  {
    printf("´ò¿ªÎÄ¼þÊ§°Ü,Ê§°Ü´úÂë:%u\r\n",res);
    atomTimerDelay (1);
  }
  printFile(&fil);
  
  f_lseek(&fil, f_size(&fil));
  f_write(&fil, writeByte, sizeof writeByte, &bw);
  if (bw != sizeof writeByte)
  {
    printf("Ð´ÈëÊ§°Ü\r\n");
    atomTimerDelay (1);
  }
  printFile(&fil);
  
  FATFS *fs1;
  DWORD fre_clust, fre_sect, tot_sect;

  /* Get volume information and free clusters of drive 1 */
  res = f_getfree("", &fre_clust, &fs1);

  /* Get total sectors and free sectors */
  tot_sect = (fs1->n_fatent - 2) * fs1->csize;
  fre_sect = fre_clust * fs1->csize;

  /* Print the free space (assuming 512 bytes/sector) */
  printf("%10lu KiB total drive space.\n%10lu KiB available.\n",
         tot_sect / 2, fre_sect / 2);
  
  /* Close the file */
  f_close(&fil);
  
  /* Unregister work area */
  f_mount(0, "", 1);

  /* Test finished, flash slowly for pass, fast for fail */
  while (1)
  {
      /* Toggle LED on pin D0 (Discovery-specific) */
      GPIO_WriteReverse(GPIOC, GPIO_PIN_1);

      /* Sleep then toggle LED again */
      atomTimerDelay (SYSTEM_TICKS_PER_SEC);
  }
}

