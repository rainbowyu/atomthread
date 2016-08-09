#ifndef __UARTRXTHREAD_H
#define __UARTRXTHREAD_H
#include "stm8s.h"
#include "atom.h"
#include "atomqueue.h"
#include "uart.h"

#define UARTPROCESS_STACK_SIZE_BYTES       400

extern ATOM_TCB uartProcess_tcb;
extern ATOM_SEM uart3Rxsem;
extern ATOM_QUEUE uartRxQueue;
extern uint8_t uartRxQueueBuff[4];
extern void uartProcess_thread_func (uint32_t param);

#endif