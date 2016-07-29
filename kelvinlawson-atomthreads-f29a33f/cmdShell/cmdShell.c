#include "cmdShell.h"
#include "mainthread.h"
#include "ctype.h"
#include "stdio.h"
#include "string.h"

#include "filethread.h"
//static void cmdCutOff(char *cmddata, char **cmd,const char *delims);
uint8_t cmdParser(Commanddata *cmddata);
int32_t stringToDec(uint8_t *buf);


int32_t stringToDec(uint8_t *buf);
uint8_t cmdParser(Commanddata *cmddata);
int32_t stringToDec(uint8_t *buf);
void handleArg(uint8_t argc,void *cmd_arg);
void printfHello(uint8_t argc,void *cmd_arg);
void showTaskHandle(uint8_t argc,void *cmd_arg);
void filelistHandle(uint8_t argc,void *cmd_arg);
void mkdirHandle(uint8_t argc,void *cmd_arg);
void rmHandle(uint8_t argc,void *cmd_arg);
void catHandle(uint8_t argc,void *cmd_arg);

const cmdListStruct CMDList[]={
  /*命令名称(cmd name)    参数数目(para num)     控制函数(cmd func)     帮助信息(help)*/
  {"hello" ,              0,                    printfHello,           "hello -打印Hello world"},
  {"showTask",            0,                    showTaskHandle,        "显示运行任务的信息"},
  {"ls",                  0,                    filelistHandle,        "显示文件目录结构"},
  {"mkdir",               1,                    mkdirHandle,           "创建文件夹"},
  {"rm",                  1,                    rmHandle,              "删除文件或文件夹"},
  {"cat",                 1,                    catHandle,             "显示文件内容"}
};

/*
  para [0] Commanddata *cmddata [in][out]
  typedef struct Commanddatas{
    uint8_t buff[CMDBUFFLEN];
    uint8_t cmdParam[ARGNUM][ARGLEN];
    uint8_t cmd[CMDLEN];
    uint8_t bufflen;   buff used size
  }Commanddata;
  return -1 too many param
  return param num
*/

uint8_t cmdParser(Commanddata *cmddata)
{
  uint32_t spaceFlag=0;          //空格标志
  uint32_t argNum=0;             //参数数目(包括cmd)
  uint32_t index[ARGNUM]={0};    //有效参数首个数字的数组索引
  uint8_t i;

  /*先做一遍分析,找出参数的数目,以及参数段的首个数字所在 cmddata->buff 数组中的下标*/
  for(uint8_t i=0;i<cmddata->bufflen;i++)
  {
    if(cmddata->buff[i]==' ')         //为空格
    {
      spaceFlag=1;
      continue;
    }
    else if(cmddata->buff[i]=='\n')   //换行
    {
      index[argNum] = i+1;            //记录结束位置 虚拟下一个参数的起始地址
      break;
    }
    else
    {
      if(spaceFlag == 1)
      {
        spaceFlag = 0;
        if(argNum < ARGNUM)
        {
          index[argNum]=i;
          argNum++;
        }
        else
        {
          return -1;                //参数个数太多
        }
      }
    }
  }

  //get cmd before first space or \n
  for (i=0;i<index[0]-1;i++)
  {
    cmddata->cmd[i]=cmddata->buff[i];
  }
  cmddata->cmd[index[0]-1]='\0';

  //get param
  for(uint8_t j=0;j<argNum;j++)
  {
    for (i=0;i<index[j+1]-index[j]-1;i++)
      cmddata->cmdParam[j][i]=cmddata->buff[index[j]+i];
    cmddata->cmdParam[j][i]='\0';
  }

  //return param num
  return argNum;
}

void printfHello(uint8_t argc,void *cmd_arg)
{
  printf("Hello world!\n");
}

void showTaskHandle(uint8_t argc,void *cmd_arg)
{
  printf("TASK NAME  |  CPU OCCUPANCY RATE  |  RAM USE  |  RAM FREE\n\r");
  for (uint8_t i=0;i<taskState.taskNum;i++)
  {
    printf("%10s   %13d    %10dByte  %7dByte\n\r",taskState.taskName[i],(int)taskState.taskCPUOcRate[i],(int)taskState.taskRAMMax[i][0],(int)taskState.taskRAMMax[i][1]);
  }
}

void filelistHandle(uint8_t argc,void *cmd_arg)
{
  fileCommandData.commandlist = LISTCOMMAND;
  atomSemPut (&fileCommondsem);
}

void mkdirHandle(uint8_t argc,void *cmd_arg)
{
  Commanddata *arg;
  arg = (Commanddata *)cmd_arg;
  fileCommandData.commandlist = CREATDIRCOMMAND;
  memcpy (fileCommandData.pathName, arg->cmdParam[0], ARGLEN);
  atomSemPut (&fileCommondsem);
}

void rmHandle(uint8_t argc,void *cmd_arg)
{
  Commanddata *arg;
  arg = (Commanddata *)cmd_arg;
  fileCommandData.commandlist = DELETECOMMAND;
  memcpy (fileCommandData.pathName, arg->cmdParam[0], ARGLEN);
  atomSemPut (&fileCommondsem);
}

void catHandle(uint8_t argc,void *cmd_arg)
{
  Commanddata *arg;
  arg = (Commanddata *)cmd_arg;
  fileCommandData.commandlist = CATCOMMAND;
  memcpy (fileCommandData.pathName, arg->cmdParam[0], ARGLEN);
  atomSemPut (&fileCommondsem);
}

int32_t stringToDec(uint8_t *buf)
{
   uint16_t i=0;
   uint8_t  base=10;       //基数
   int8_t   neg=1;         //表示正负,1=正数
   int32_t  result=0;

   if((buf[0]=='0')&&(buf[1]=='x'))
   {
     base=16;
     neg=1;
     i=2;
   }
   else if(buf[0]=='-')
   {
     base=10;
     neg=-1;
     i=1;
   }
   for(;;)
   {
     if(buf[i]=='\0')                    //为空
     {
       break;
     }

     result *= base;
     if(isdigit(buf[i]))                 //是否为0~9
     {
       result += buf[i]-'0';
     }
     else if(isxdigit(buf[i]))           //是否为a~f或者A~F
     {
       result+=tolower(buf[i])-87;
     }
     else
     {
       result += buf[i]-'0';
     }
   }
   result *= neg;
   return result ;
}
