#ifndef __UARTRXTHREAD_H
#define __UARTRXTHREAD_H
#include "stm8s.h"
#define UARTPROCESS_STACK_SIZE_BYTES       400

extern ATOM_TCB uartProcess_tcb;
extern ATOM_SEM uartRxsem;

extern void uartProcess_thread_func (uint32_t param);

#endif