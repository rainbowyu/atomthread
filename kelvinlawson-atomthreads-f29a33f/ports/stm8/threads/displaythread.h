#ifndef __DISPLAYTHREAD_H
#define __DISPLAYTHREAD_H
#include "stm8s.h"

#define DISPLAY_STACK_SIZE_BYTES           1500
//display command
#define NEWCOMMAND 0x01

typedef struct disCommanddatas{
  uint16_t commandlist;
  uint8_t buff[50];
}disComdata;
extern disComdata disCommandData;
extern ATOM_TCB display_tcb;
extern ATOM_SEM disCommondsem;

extern void display_thread_func (uint32_t param);
#endif