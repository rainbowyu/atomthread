#ifndef __CMDSHELL_H
#define __CMDSHELL_H
#include "stm8s.h"

#define CMDBUFFLEN 50         //命令缓存长度
#define ARGNUM     5+1        //命令中允许的参数个数
#define ARGLEN     15         //命令中允许的参数长度
#define CMDLEN     10         //命令名占用的最大字符长度

#define SHELLCMDNUM 6
typedef struct Commanddatas{
  uint8_t buff[CMDBUFFLEN];
  uint8_t cmdParam[ARGNUM][ARGLEN];
  uint8_t cmd[CMDLEN];
  uint8_t bufflen;
}Commanddata;

typedef struct {
     char const *cmd_name;                            //命令字符串
     int32_t max_args;                                //最大参数数目
     void (*handle)(uint8_t argc,void * cmd_arg);     //命令回调函数
     char  *help;                                     //帮助信息
 }cmdListStruct;

extern const cmdListStruct CMDList[];

extern Commanddata cmdData;

extern int32_t stringToDec(uint8_t *buf);
extern uint8_t cmdParser(Commanddata *cmddata);
extern int32_t stringToDec(uint8_t *buf);
extern void handleArg(uint8_t argc,void *cmd_arg);
extern void filelistHandle(uint8_t argc,void *cmd_arg);
extern void showTaskHandle(uint8_t argc,void *cmd_arg);
extern void printfHello(uint8_t argc,void *cmd_arg);
extern void mkdirHandle(uint8_t argc,void *cmd_arg);
extern void rmHandle(uint8_t argc,void *cmd_arg);
extern void catHandle(uint8_t argc,void *cmd_arg);
#endif
