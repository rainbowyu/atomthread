#ifndef __FILETHREAD_H
#define __FILETHREAD_H
#include "stm8s.h"

#define FILE_STACK_SIZE_BYTES           1500

//file command
#define CREATCOMMAND  0x0001
#define OPENCOMMAND   0x0002
#define READCOMMAND   0x0004
#define WRITECOMMAND  0x0008

typedef struct fileCommanddatas{
  uint16_t commandlist;
  uint8_t buff[50];
}fileComdata;

extern fileComdata fileCommandData;
extern ATOM_TCB file_tcb;
extern ATOM_SEM fileCommondsem;

extern void file_thread_func (uint32_t param);
#endif