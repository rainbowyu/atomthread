#ifndef __MAINTHREAD_H
#define __MAINTHREAD_H

//IDEL 线程堆栈大小 根据需求更改 最小128
#define IDLE_STACK_SIZE_BYTES       256

#define TASKNAMELENMAX 10
#define TASKMAX 5

typedef struct taskstate{
  uint8_t  taskName[TASKMAX][TASKNAMELENMAX];
  uint16_t taskRAMMax[TASKMAX][2];            //free and use
  uint8_t  taskCPUOcRate[TASKMAX];
  uint8_t  taskNum;
}taskStateStruct;

extern taskStateStruct taskState;
#endif