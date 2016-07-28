#ifndef __CMDSHELLTHREAD_H
#define __CMDSHELLTHREAD_H
#include "stm8s.h"
#include "cmdShell.h"
#define CMDSHELL_STACK_SIZE_BYTES           500

extern Commanddata cmdData;

extern ATOM_TCB cmdshell_tcb;
extern ATOM_SEM cmdShellsem;

extern void cmdshell_thread_func (uint32_t param);
#endif
