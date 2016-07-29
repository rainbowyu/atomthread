#include "stdio.h"
#include "string.h"

#include "stm8s.h"
#include "u8g.h"
#include "u8g_stm8s.h"
#include "ff.h"

#include "atom.h"
#include "atomtimer.h"
#include "atomsem.h"
#include "atommutex.h"

#include "filethread.h"
#include "spiflash.h"
#include "filecommon.h"
#include "mainthread.h"

ATOM_TCB file_tcb;

ATOM_SEM fileCommondsem;
fileComdata fileCommandData;

void file_thread_func (uint32_t param);

static void fileCmdProcess(fileComdata* cmd,uint8_t *buff);

void file_thread_func (uint32_t param)
{
  FATFS fs;           /* File system object */
  FRESULT res;        /* API result code */
  uint8_t readByte[50];
  volatile uint32_t use=0,free=0;
  
  
  res=f_mount(&fs, "", 1);
  if (res)
  {
    printf("¹ÒÔØÊ§°Ü,Ê§°Ü´úÂë:%u\r\n",res);
  }
  
  while (1)
  {
    if (atomSemGet (&fileCommondsem, 0) == ATOM_OK)
    {
      fileCmdProcess(&fileCommandData,readByte);
      
      //get the used RAM
      atomThreadStackCheck (&file_tcb, (uint32_t*)&use, (uint32_t*)&free);
      taskState.taskRAMMax[file_tcb.threadNum][0]=(uint16_t)use;
      taskState.taskRAMMax[file_tcb.threadNum][1]=(uint16_t)free;
    }
  }
}

static void fileCmdProcess(fileComdata* cmd,uint8_t *buff)
{
  static FIL fil;            // File object 
  static DIR dirp;
  static uint8_t patchNow[20]="/";
  UINT brw;
  FRESULT res;
  switch (cmd->commandlist)
  {
    case CREATCOMMAND:
      res = f_open(&fil, (const char *)cmd->pathName, FA_CREATE_NEW);
      if (res != FR_OK)
        printf("open file %s failed\n",(const char *)cmd->pathName);
      f_close(&fil);
      break;

    case DELETECOMMAND:
      res = f_unlink ((const char *)cmd->pathName);
      if (res != FR_OK)
        printf("delete file or dir %s failed\n",(const char *)cmd->pathName);
      else 
      {
        printf("delete file or dir %s done\n",(const char *)cmd->pathName);
      }
      break;
      
    case CATCOMMAND:
      res = f_open(&fil, (const char *)cmd->pathName, FA_READ);
      if (res != FR_OK)
        printf("cat file %s failed\n",(const char *)cmd->pathName);
      else printFile(&fil);
      f_close(&fil);
      break;

    case READCOMMAND:
      res = f_read(&fil, buff, f_size(&fil), &brw);
      if (res != FR_OK || brw != f_size(&fil))
        printf("read file %s failed\n",(const char *)cmd->pathName);
      break;

    case WRITECOMMAND:
      res = f_write(&fil, (const char *)buff, cmd->buffsize, &brw);
      if (res != FR_OK || brw != cmd->buffsize)
        printf("write file %s failed\n",(const char *)cmd->pathName);
      break;

    case LISTCOMMAND:
      printf("---------------------------------\n");
      res = scan_files((char*)patchNow);
      if (res != FR_OK)
        printf("scan  %s failed, error code %d\n\r",(const char *)patchNow,(int)res);
      printf("---------------------------------\n");
      break;

    case OPENDIRCOMMAND:
      res = f_opendir(&dirp, (const char *)cmd->pathName);
      memcpy (patchNow, cmd->pathName, 20);
      if (res != FR_OK)
        printf("open dir %s failed",(const char *)cmd->pathName);
      break;

    case CLOSEFILECOMMAND:
      res = f_close (&fil);
      if (res != FR_OK)
        printf("close %s failed",(const char *)cmd->pathName);
      break;

    case CLOSEDIRCOMMAND:
      res = f_closedir (&dirp);
      if (res != FR_OK)
        printf("close dir %s failed",(const char *)cmd->pathName);
      break;
      
    case CREATDIRCOMMAND:
      res = f_mkdir((const char*)cmd->pathName);
      if (res != FR_OK)
        printf("make dir %s failed, error code %d \n\r",(const char *)cmd->pathName,(int)res);
    default:
      break;
  }
}
