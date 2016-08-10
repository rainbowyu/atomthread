#ifdef GPS
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "stdlib.h"

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

const forbidCentre centre[2]=
{
  /*  lat       lon     distance */
  {39.906913,116.384305,  40.0   },
  {20.906913,106.384305,  40.0   },
};

ATOM_TCB gpsProcess_tcb;
ATOM_SEM gpsDatasem;
ATOM_MUTEX gpsDatamutex;

GPSdata GPSData;

void gpsProcess_thread_func (uint32_t param);
double GPStoDistance(double lat1,double lon1,double lat2,double lon2);
void GPSDataProcess(GPSdata *gps);
void GPSDataCut(GPSdata *gps);
int8_t stringToInt(uint8_t scale, char * string, uint32_t *intNum);
int8_t GPSCheckSum(char *gpsData);
int8_t stringTofloat(char * string, double *intNum);
int8_t GPSLatProcess(double *lat,char dir);
int8_t GPSLonProcess(double *lon,char dir);

void gpsProcess_thread_func (uint32_t param)
{
  volatile uint32_t use=0,free=0;
  
  while (1)
  {
    //wait forever
    if (atomSemGet(&gpsDatasem, 0) == ATOM_OK)
    {   
      
      if ( GPSCheckSum ((char *)GPSData.buff)==0 )
      {
        atomMutexGet (&gpsDatamutex,0);
        GPSDataCut(&GPSData);
        GPSDataProcess(&GPSData);
        atomMutexPut (&gpsDatamutex);
      }
       
      //get the used RAM
      atomThreadStackCheck (&gpsProcess_tcb, (uint32_t*)&use, (uint32_t*)&free);
      taskState.taskRAMMax[gpsProcess_tcb.threadNum][0]=(uint16_t)use;
      taskState.taskRAMMax[gpsProcess_tcb.threadNum][1]=(uint16_t)free;
    }
  }
}

int8_t stringToInt(uint8_t scale, char * string, uint32_t *intNum)
{
  char *p=NULL;
  int8_t state=0;
  switch (scale)
  {
    case 2:;
    case 8:;
    case 10:
      for (p = string;(*p) != '\0';p++)
      {
        *intNum = (*intNum) * scale;
        *intNum += (*p)-'0';
      }
      break;
    case 16:
      for (p = string;(*p) != '\0';p++)
      {
        *intNum = (*intNum) * 16;
        //大写转小写
        if ((*p)>='A'&&(*p)<='F')
        {*p = *p + 32;}
        if((*p)>='a'&&(*p)<='f')
        {(*intNum) += ((*p)-87);}
        else
        {(*intNum) += ((*p)-'0');}
      }
      break;
    default:
      state = -1;
      break;
  }
  return state;
}

int8_t stringTofloat(char * string, double *intNum)
{
  char *p=NULL;
  int8_t state=0;
  uint32_t temp;
  double temp1;
  uint8_t i=0;
  uint8_t flag=0;
  for (p = string;(*p) != '\0';p++)
  {
    if (flag)
    {
      i++;
      temp1 = temp1 * 10.0;
      temp1 += (*p)-'0';
    }
    else 
    {
      temp = temp * 10;
      if ((*p)=='.')
      {
        flag = 1;
      }
      else
      {
        temp += (*p)-'0';
      }
    }
  }
  do
  {
    temp1=temp1/10.0;
  }while(i--);
  *intNum= (double)temp + temp1;
  if (!flag)state = -1;
  return state;
}

/**
 *  GPS定位信息 $GPGGA (1)，(2)，(3)，(4)，(5)，(6)，(7)，(8)，(9)，M，(10)，M，(11)，(12)＊(13)(CR)(LF)
 *  例：$GPGGA,092204.999,4000.2330,N,11603.2365,E,1,04,24.4,19.7,M,,,,0000*03
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
 *  GPS数据剪切
 */

int8_t GPSCheckSum(char *gpsData)
{
  int8_t state=0;
  uint8_t result=0;
  uint32_t checkSum=0;
  char charSum[3]={0};
  char *p=NULL;
  
  //去掉$
  for (p=gpsData+1;(*p)!='*';p++)
  {
    result ^= *p;
  }
  charSum[0]=*(p+1);
  charSum[1]=*(p+2);
  stringToInt(16, charSum, &checkSum);
  if (checkSum != result)state=-1;
  return state;
}


/**
 *  GPS数据剪切
 */
void GPSDataCut(GPSdata *gps)
{
  char *gpsCuttemp;
  uint8_t i=0;
  const char *split = ",";
  char bufftemp[GPSBUFFLEN];
  strncpy(bufftemp , (char*)gps->buff, GPSBUFFLEN);
  //p=gpsCuttemp;
  
  //gpsCut[i] = strtok ((char*)gpsData,split);
  for(gpsCuttemp=strtok ( bufftemp, split);gpsCuttemp!=NULL;i++) 
  {
    strncpy(gps->gpsCut[i], gpsCuttemp, 11);
    gpsCuttemp = strtok(NULL,split);
  }
}

/**
 *  GPS数据处理
 *  参数1[in] gpsCut数据,由逗号切割
 *  参数2[out] lat 纬度 <0南纬(S) >0北纬(N)
 *  参数3[out] lon 经度 <0西经(W) >0东经(E)
 */
double distance;
uint32_t temp;
void GPSDataProcess(GPSdata *gps)
{
  static uint32_t gpsReadCount=0; 
  if (strcmp((const char *)gps->gpsCut[0], "$GPGGA") == 0)
  {
    //stringTofloat(gps->gpsCut[1], &kk);
    gps->UTC=atof(gps->gpsCut[1]);
    gps->lat=atof(gps->gpsCut[2]);
    GPSLatProcess(&gps->lat,gps->gpsCut[3][0]);
    gps->lon=atof(gps->gpsCut[4]);
    GPSLonProcess(&gps->lon,gps->gpsCut[5][0]);
    stringToInt(10, gps->gpsCut[6], &temp);
    gps->GPSQuality = (uint8_t)temp;
    temp=0;
    stringToInt(10, gps->gpsCut[7], &temp);
    gps->sateNum = (uint8_t)temp;
    gps->Horiprc=atof(gps->gpsCut[8]);
    gps->AntAlt=atof(gps->gpsCut[9]);   
    distance = GPStoDistance(centre[0].lat,centre[0].lon,gps->lat,gps->lon);
    if (gps->GPSQuality)
    {
      gpsReadCount++;
    }
    if (gpsReadCount>50)
    {
      if (distance < centre[0].distance || 0)
      {
        for (uint8_t i=0;i<GPSBUFFLEN;i++)
        {
          if (gps->buff[i]=='N')gps->buff[i]='S';
          if (gps->buff[i]=='S')gps->buff[i]='N';
        }
      }
    }
    printf("%s",gps->buff);
  }
}


//lat lon 经度 <0西经(W) >0东经(E)
int8_t GPSLatProcess(double *lat,char dir)
{
  double temp,temp1;
  uint32_t i;
  temp = *lat;
  temp1 = *lat;
  
  temp /= 100.0;
  i = (uint32_t)temp;
  
  temp1 -= i*100;
  temp1 /= 60.0;
  switch (dir)
  {
    case 'W':
      *lat = -(i+temp1);
      break;
    case 'E':
      *lat = i+temp1;
      break;
  }
  return 0;
}

//lat 经度 <0南纬(S) >0北纬(N)
int8_t GPSLonProcess(double *lon,char dir)
{
  double temp,temp1;
  uint32_t i;
  temp = *lon;
  temp1 = *lon;
  
  temp /= 100.0;
  i = (uint32_t)temp;
  
  temp1 -= i*100;
  temp1 /= 60.0;
  switch (dir)
  {
    case 'S':
      *lon = -(i+temp1);
      break;
    case 'N':
      *lon = i+temp1;
      break;
  }
  return 0;
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
  double dist = theta * EARTH_RADIUS;
  return dist;
}
#endif 
