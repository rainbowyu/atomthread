#ifndef __FILETHREAD_H
#define __FILETHREAD_H
#include "stm8s.h"
#include "atom.h"
#include "atomsem.h"
#define FILE_STACK_SIZE_BYTES           2000

//file command
#define CREATCOMMAND     0x0001
#define OPENCOMMAND      0x0002
#define READCOMMAND      0x0004
#define WRITECOMMAND     0x0008
#define LISTCOMMAND      0x0010
#define OPENDIRCOMMAND   0x0020
#define CLOSEFILECOMMAND 0x0040
#define CLOSEDIRCOMMAND  0x0080

typedef struct fileCommanddatas{
  uint16_t commandlist;
  char name[20];
  uint8_t buff[50];
  uint8_t buffsize;
}fileComdata;

extern fileComdata fileCommandData;
extern ATOM_TCB file_tcb;
extern ATOM_SEM fileCommondsem;

extern void file_thread_func (uint32_t param);
#endif
