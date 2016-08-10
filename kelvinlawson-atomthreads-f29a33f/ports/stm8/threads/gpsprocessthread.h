#ifndef __GPSPROCESSTHREAD_H
#define __GPSPROCESSTHREAD_H
#include "stm8s.h"
#include "atom.h"
#include "atomqueue.h"
#include "uart.h"

#define GPSPROCESS_STACK_SIZE_BYTES       1000
#define GPSBUFFLEN RXBUFFLEN

typedef struct GpsDatas{
  uint8_t buff[GPSBUFFLEN];         //数据缓存
  char gpsCut[15][11];              //gps数据分割
  double UTC;                       //UTC时间
  double lat;                       //纬度
  double lon;                       //经度
  uint8_t sateNum;                  //卫星数量 0-12
  uint8_t GPSQuality;               //卫星质量 
  double Horiprc;                   //水平精度因子
  double AntAlt;                    //天线海拔
}GPSdata;

typedef struct centre{
  double lat;                       //纬度
  double lon;                       //经度
  double distance;
}forbidCentre;


extern GPSdata GPSData;
extern ATOM_TCB gpsProcess_tcb;
extern ATOM_SEM gpsDatasem;
extern ATOM_MUTEX gpsDatamutex;
extern void gpsProcess_thread_func (uint32_t param);
extern double GPStoDistance(double lat1,double lon1,double lat2,double lon2);
extern void GPSProcess(double *lat,double *lon,uint8_t *gpsData,char gpsCut[][11]);

#endif