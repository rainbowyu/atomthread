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
static void fileCmdProcess(fileComdata *cmd);

void file_thread_func (uint32_t param)
{
  FATFS fs;           /* File system object */
  FRESULT res;        /* API result code */

  res=f_mount(&fs, "", 1);
  if (res)
  {
    printf("¹ÒÔØÊ§°Ü,Ê§°Ü´úÂë:%u\r\n",res);
  }
  scan_files ((char*)"/");
  while (1)
  {
    if (atomSemGet (&fileCommondsem, 0) == ATOM_OK)
    {
      fileCmdProcess(&fileCommandData);
    }
  }
}

static void fileCmdProcess(fileComdata* cmd,uint8_t *buff)
{
  static FIL fil;            /* File object */
  static DIR dirp;
  UINT brw;
  FRESULT res;
  switch (cmd->commandlist)
  {
    case CREATCOMMAND:
      res = f_open(&fil, (const char *)cmd->name, FA_CREATE_NEW);
      if (res != FR_OK)
        printf("open file %s failed",(const char *)cmd->name);
      break;

    case OPENCOMMAND:

      break;

    case READCOMMAND:
    //f_size(file)< display buff
      res = f_read(&fil, buff, f_size(file), &brw);
      if (res != FR_OK || brw != f_size(file))
        printf("read file %s failed",(const char *)cmd->name);
      break;

    case WRITECOMMAND:
      res = f_write(&fil, (const char *)buff, cmd->buffsize, &brw);
      if (res != FR_OK || brw != cmd->buffsize)
        printf("write file %s failed",(const char *)cmd->name);
      break;

    case LISTCOMMAND:
      scan_files((const char *)cmd->name);
      break;

    case OPENDIRCOMMAND:
      res = f_opendir(&dirp, (const char *)cmd->name);
      if (res != FR_OK)
        printf("open dir %s failed",(const char *)cmd->name);
      break;

    case CLOSEFILECOMMAND:
      res = f_close (&fil);
      if (res != FR_OK)
        printf("close %s failed",(const char *)cmd->name);
      break;

    case CLOSEDIRCOMMAND:
      res = f_closedir (&dirp);
      if (res != FR_OK)
        printf("close dir %s failed",(const char *)cmd->name);
      break;

    default:
      break;
  }
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
