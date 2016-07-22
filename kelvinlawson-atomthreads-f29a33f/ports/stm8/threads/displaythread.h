#ifndef __DISPLAYTHREAD_H
#define __DISPLAYTHREAD_H
#include "stm8s.h"

typedef struct disCommanddatas{
  uint16_t commandlist;
  uint8_t buff[50];
}disComdata;

#define DISPLAY_STACK_SIZE_BYTES           1500
#define NEWCOMMAND 0x01
extern void display_thread_func (uint32_t param);

#endif