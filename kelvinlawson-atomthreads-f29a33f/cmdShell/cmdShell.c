#include "cmdShell.h"
#include "string.h"
Commanddata cmdData;

static void cmdCutOff(char *cmddata, char **cmd,const char *delims);
void cmdparser(Commanddata *cmddata);

void cmdparser(Commanddata *cmddata)
{
  //10 word per spaces
  char command[5][10];
  //分割字符 保存在command中
  cmdCutOff(cmddata->buff, command," ");
}

static void cmdCutOff(char *cmddata, char **cmd,const char *delims)
{
  //开始分割
  char *result = NULL;
  uint8_t i;
  result = strtok( cmddata->buff, (char *)delims );
  while( result != NULL ) {
    cmd[i] = result ;
    i++;
    printf( "result is \"%s\"\n", result );
    result = strtok( NULL, delims );
  }
}
