#include "stdio.h"
#include "string.h"

#include "stm8s.h"
#include "u8g.h"
#include "u8g_stm8s.h"
#include "ff.h"


#include "atom.h"
#include "atomtimer.h"
#include "atomsem.h"
#include "atomqueue.h"
#include "atommutex.h"

#include "filethread.h"

ATOM_TCB file_tcb;

ATOM_SEM fileCommondsem;
fileComdata fileCommandData;

void file_thread_func (uint32_t param);
FRESULT scan_files (char* path);
static void fileCmdParser(fileComdata *cmd);

void file_thread_func (uint32_t param)
{
  FATFS fs;           /* File system object */
  //FIL fil;            /* File object */
  FRESULT res;        /* API result code */
  //UINT bw;            /* Bytes written */
  
  res=f_mount(&fs, "", 1);
  if (res)
  {
    printf("π“‘ÿ ß∞‹, ß∞‹¥˙¬Î:%u\r\n",res);
  }
  scan_files ((char*)"/");
  while (1)
  {
    if (atomSemGet (&fileCommondsem, 0) == ATOM_OK)
    {
      fileCmdParser(&fileCommandData);
    }
  }
} 

static void fileCmdParser(fileComdata* cmd)
{
  
}

FRESULT scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;

    res = f_opendir(&dir, path);                           /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                res = scan_files(path);                    /* Enter the directory */
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                printf("%s/%s\n", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }
    return res;
}
