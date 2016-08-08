#include "stdio.h"
#include "math.h"

#include "stm8s.h"
#include "uart.h"

#include "atom.h"
#include "atomtimer.h"
#include "atomsem.h"
#include "atommutex.h"

#include "uartrxthread.h"
#include "cmdshellthread.h"
#include "mainthread.h"
//#include "displaythread.h"
#define pi 3.14159
#define EARTH_RADIUS 6378137
ATOM_TCB uartProcess_tcb;
ATOM_SEM uartRxsem;

void uartProcess_thread_func (uint32_t param);

void uartProcess_thread_func (uint32_t param)
{
  volatile uint32_t use=0,free=0;
  double lat1=39.906913,lat2=39.999844,lon1=116.384305,lon2=116.018330;
  double radLat1 = lat1 * pi /180.0;  
  double radLat2 = lat2 * pi /180.0;  
  
  double radLon1 = lon1 * pi /180.0;  
  double radLon2 = lon2 * pi /180.0;  
  
  if (radLat1 < 0)  
      radLat1 = pi / 2 + abs(radLat1);// south  
  if (radLat1 > 0)  
      radLat1 = pi / 2 - abs(radLat1);// north  
  if (radLon1 < 0)  
      radLon1 = pi * 2 - abs(radLon1);// west  
  if (radLat2 < 0)  
      radLat2 = pi / 2 + abs(radLat2);// south  
  if (radLat2 > 0)  
      radLat2 = pi / 2 - abs(radLat2);// north  
  if (radLon2 < 0)  
      radLon2 = pi * 2 - abs(radLon2);// west  
  double x1 = EARTH_RADIUS * cos(radLon1) * sin(radLat1);  
  double y1 = EARTH_RADIUS * sin(radLon1) * sin(radLat1);  
  double z1 = EARTH_RADIUS * cos(radLat1);  
  
  double x2 = EARTH_RADIUS * cos(radLon2) * sin(radLat2);  
  double y2 = EARTH_RADIUS * sin(radLon2) * sin(radLat2);  
  double z2 = EARTH_RADIUS * cos(radLat2);  
  
  double d = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)+ (z1 - z2) * (z1 - z2));  
  //ÓàÏÒ¶¨ÀíÇó¼Ð½Ç  
  double theta = acos((EARTH_RADIUS * EARTH_RADIUS + EARTH_RADIUS * EARTH_RADIUS - d * d) / (2 * EARTH_RADIUS * EARTH_RADIUS));  
  double dist = theta * EARTH_RADIUS;  
  printf ("dist:%lf",dist);

  while (1)
  {
    //wait forever
    if (atomSemGet(&uartRxsem, 0) == ATOM_OK)
    {
//      //UART send data to cli
//      for (uint8_t i=0;i<rxDataBuff.len;i++)
//      {
//        cmdData.buff[i] = rxDataBuff.buff[i];
//      }
//      cmdData.bufflen = rxDataBuff.len;
//      atomSemPut (&cmdShellsem);
//      
//      //get the used RAM
//      atomThreadStackCheck (&uartProcess_tcb, (uint32_t*)&use, (uint32_t*)&free);
//      taskState.taskRAMMax[uartProcess_tcb.threadNum][0]=(uint16_t)use;
//      taskState.taskRAMMax[uartProcess_tcb.threadNum][1]=(uint16_t)free;
    }
  }
}
