#ifdef GPS
#include "stdio.h"
#include "string.h"
#include "math.h"

#include "stm8s.h"
#include "uart.h"

#include "atom.h"
#include "atomtimer.h"
#include "atomsem.h"
#include "atommutex.h"

#include "gpsprocessthread.h"
#include "mainthread.h"

#define pi 3.14159
#define EARTH_RADIUS 6378.137
#define rad(x) (((x)*(pi))/(180.0))

ATOM_TCB gpsProcess_tcb;
ATOM_SEM gpsDatasem;
ATOM_MUTEX gpsDatamutex;

GPSdata GPSData;

double lat1=39.906913,lat2=39.999844,lon1=116.384305,lon2=116.018330;
double dist;

void gpsProcess_thread_func (uint32_t param);
double GPStoDistance(double lat1,double lon1,double lat2,double lon2);
void GPSProcess(double *lat,double *lon,uint8_t *gpsData,char gpsCut[][11]);

char gpsCut[15][11];

void gpsProcess_thread_func (uint32_t param)
{
  volatile uint32_t use=0,free=0;
  double a,b;
  
  while (1)
  {
    //wait forever
    if (atomSemGet(&gpsDatasem, 0) == ATOM_OK)
    {   
      atomMutexGet (&gpsDatamutex,0);
      GPSProcess (&a,&b,GPSData.buff,gpsCut);
      atomMutexPut (&gpsDatamutex);
      
      //get the used RAM
      atomThreadStackCheck (&gpsProcess_tcb, (uint32_t*)&use, (uint32_t*)&free);
      taskState.taskRAMMax[gpsProcess_tcb.threadNum][0]=(uint16_t)use;
      taskState.taskRAMMax[gpsProcess_tcb.threadNum][1]=(uint16_t)free;
    }
  }
}

/**
 *  GPS定位信息 $GPGGA (1)，(2)，(3)，(4)，(5)，(6)，(7)，(8)，(9)，M，(10)，M，(11)，(12)＊(13)(CR)(LF)
 *  例：$GPGGA,092204.999,4250.5589,S,14718.5084,E,1,04,24.4,19.7,M,,,,0000*1F
 *  (1)定位UTC时间：05时09分01秒　　 
 *  (2)纬度(格式ddmm.mmmm:即dd度，mm.mmmm分)；　　 
 *  (3)N/S(北纬或南纬)：北纬39度31.4449分；　　 
 *  (4)经度(格式dddmm.mmmm：即ddd度，mm.mmmm分)；　　 
 *  (5)E/W(东经或西经)：东经116度43.5123分；　　 
 *  (6)质量因子(0=没有定位，1=实时GPS，2=差分GPS)：1=实时GPS；　　 
 *  (7)可使用的卫星数(0～8)：可使用的卫星数=07；　　 
 *  (8)水平精度因子(1.0～99.9)；水平精度因子=1.4；　　 
 *  (9)天线高程(海平面，－9999.9～99999.9，单位：m)；天线高程=76.2m); 　　 
 *  (10)大地椭球面相对海平面的高度(－999.9～9999.9，单位：m):－7.0m; 　　 
 *  (11)差分GPS数据年龄，实时GPS时无:无; 　　 
 *  (12)差分基准站号(0000～1023)，实时GPS时无:无;
 *  (13)校验值.

 *  GPS地理位置 $GPGLL (1)，(2)，(3)，(4)，(5)，(6)*(7)
 *  例：$GPGLL,4250.5589,S,14718.5084,E,092204.999,A*2D
 *  (0)$GPGLL，语句ID，表明该语句为Geographic Position（GLL）地理定位信息
 *  (1)纬度ddmm.mmmm，度分格式（前导位数不足则补0）
 *  (2)纬度N（北纬）或S（南纬）
 *  (3)经度dddmm.mmmm，度分格式（前导位数不足则补0）
 *  (4)经度E（东经）或W（西经）
 *  (5)UTC时间，hhmmss.sss格式
 *  (6)状态，A=定位，V=未定位
 *  (7)校验值

 *  $GPGSA

 *  $GPGSV

 *  推荐最小导航信息 $GPRMC (1)，(2)，(3)，(4)，(5)，(6)，(7)，(8)，(9)，M，(10)，M，(11)，(12)
 *  例：$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50
 *  (0)：$GPRMC，语句ID，表明该语句为Recommended Minimum Specific GPS/TRANSIT Data（RMC）推荐最小定位信息
 *  (1)：UTC时间，hhmmss.sss格式
 *  (2)：状态，A=定位，V=未定位
 *  (3)：纬度ddmm.mmmm，度分格式（前导位数不足则补0）
 *  (4)：纬度N（北纬）或S（南纬）
 *  (5)：经度dddmm.mmmm，度分格式（前导位数不足则补0）
 *  (6)：经度E（东经）或W（西经）
 *  (7)：速度，节，Knots
 *  (8)：方位角，度
 *  (9)：UTC日期，DDMMYY格式
 *  (10)：磁偏角，（000 - 180）度（前导位数不足则补0）
 *  (11)：磁偏角方向，E=东W=西
 *  (12)：校验值

 *  $GPVTG

 */

/**
 *  GPS数据解析
 */
void GPSProcess(double *lat,double *lon,uint8_t *gpsData,char gpsCut[][11])
{
  char *gpsCuttemp;
  uint8_t i=0;
  const char *split = ",";
  //p=gpsCuttemp;
  
  //gpsCut[i] = strtok ((char*)gpsData,split);
  for(gpsCuttemp=strtok ((char*)gpsData,split);gpsCuttemp!=NULL;i++) 
  {
    strncpy(gpsCut[i], gpsCuttemp, 11);
    gpsCuttemp = strtok(NULL,split);
  }
}

/**
 *  返回2点距离(km)
 *  lat 纬度 <0南纬(S) >0北纬(N)
 *  lon 经度 <0西经(W) >0东经(E)
 */
double GPStoDistance(double lat1,double lon1,double lat2,double lon2)
{
  lat1 = rad(lat1);
  lat2 = rad(lat2);

  lon1 = rad(lon1);
  lon1 = rad(lon2);
  
  if (lat1 < 0)  
      lat1 = pi / 2 + fabs(lat1);// south
  if (lat1 > 0)
      lat1 = pi / 2 - fabs(lat1);// north
  if (lon1 < 0)
      lon1 = pi * 2 - fabs(lon1);// west
  if (lat2 < 0)
      lat2 = pi / 2 + fabs(lat2);// south
  if (lat2 > 0)
      lat2 = pi / 2 - fabs(lat2);// north  
  if (lon2 < 0)  
      lon2 = pi * 2 - fabs(lon2);// west  
  double x1 = EARTH_RADIUS * cos(lon1) * sin(lat1);  
  double y1 = EARTH_RADIUS * sin(lon1) * sin(lat1);  
  double z1 = EARTH_RADIUS * cos(lat1);

  double x2 = EARTH_RADIUS * cos(lon2) * sin(lat2);  
  double y2 = EARTH_RADIUS * sin(lon2) * sin(lat2);  
  double z2 = EARTH_RADIUS * cos(lat2);  
  
  double d = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)+ (z1 - z2) * (z1 - z2));  
  //余弦定理求夹角  
  double theta = acos((EARTH_RADIUS * EARTH_RADIUS + EARTH_RADIUS * EARTH_RADIUS - d * d) / (2 * EARTH_RADIUS * EARTH_RADIUS));  
  dist = theta * EARTH_RADIUS;
  return dist;
}
#endif 
