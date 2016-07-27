/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "stdio.h"
//#include "spiffs_config.h"
#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_spi.h"
#include "atomtimer.h"
#include "atom.h"
#include "spiflash.h"

/* Definitions of physical drive number for each drive */
#define DEV_MMC		0	/* Example: Map Ramdisk to physical drive 0     */
#define DEV_RAM		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2     */

/*内部函数*/
static int MMC_disk_status();
static int MMC_disk_initialize();
static int MMC_disk_write(const BYTE *buff, DWORD sector, UINT count);
static int MMC_disk_read(BYTE *buff, DWORD sector, UINT count);

static int RAM_disk_status();
static int RAM_disk_initialize();
static int RAM_disk_write(const BYTE *buff, DWORD sector, UINT count);
static int RAM_disk_read(BYTE *buff, DWORD sector, UINT count);

static int USB_disk_status();
static int USB_disk_initialize();
static int USB_disk_write(const BYTE *buff, DWORD sector, UINT count);
static int USB_disk_read(BYTE *buff, DWORD sector, UINT count);

/*flash 操作函数*/
u8 spiSendByte( u8 byteValue );

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		            /* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat=RES_OK;
	//int result=0;

	switch (pdrv) {
	case DEV_RAM :
		RAM_disk_status();
		return stat;

	case DEV_MMC :
		MMC_disk_status();
		return stat;

	case DEV_USB :
		USB_disk_status();
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat=RES_OK;
//  int result=0;
	switch (pdrv) {
	case DEV_RAM :
		RAM_disk_initialize();
		return stat;

	case DEV_MMC :
		MMC_disk_initialize();
		return stat;

	case DEV_USB :
		USB_disk_initialize();
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res=RES_OK;
  
	switch (pdrv) {
	case DEV_RAM :
		RAM_disk_read(buff, sector, count);
		return res;

	case DEV_MMC :
		MMC_disk_read(buff, sector, count);
		return res;

	case DEV_USB :
		USB_disk_read(buff, sector, count);
		return res;
	}

	return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,		    /* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		  /* Start sector in LBA */
	UINT count		    /* Number of sectors to write */
)
{
	DRESULT res =RES_OK;
  
	switch (pdrv) {
	case DEV_RAM :
		RAM_disk_write(buff, sector, count);
		return res;

	case DEV_MMC :
		MMC_disk_write(buff, sector, count);
		return res;

	case DEV_USB :
		USB_disk_write(buff, sector, count);
		return res;
	}
	return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res=RES_OK;

	switch (pdrv) {
	case DEV_RAM :
    switch (cmd)
    {
     
        
    }
		// Process of the command for the RAM drive

		return res;

	case DEV_MMC :
    
    switch (cmd)
    {

      case GET_BLOCK_SIZE:
        *(DWORD*)buff = 1;//可擦除的最小单位大小
        break;
      
      case GET_SECTOR_SIZE:
        *(DWORD*)buff = 512;
        break;
      
      case GET_SECTOR_COUNT:
        *(DWORD*)buff = 0x200;
        break;
        
    }
		return res;

	case DEV_USB :
		return res;
	}

	return RES_PARERR;
}

static int MMC_disk_status()
{
  return 0;
}
/*
 * 初始化SPI:
 * 先发送MSB,
 * 波特率4分频,
 * 主模式,
 * 空闲状态SCK保持低,
 * 数据采样从第一个时钟边沿开始,
 * 双线单项数据模式,
 * 禁止软件从设备管理,
 * crc校验0x00
 */
static int MMC_disk_initialize()
{
  spiFlashInit();
  return RES_OK;
}

static int MMC_disk_write(const BYTE *buff, DWORD sector, UINT count)
{
	BYTE *buf = (BYTE*)buff;       
	for(UINT i=0; i < count; i++){
		flashEraseSector(sector);
    spiFlashWriteSector(sector, buf);
    sector ++;
		buf += FLASH_SECTOR_SIZE;
	}
  return RES_OK;
}

static int MMC_disk_read(BYTE *buff, DWORD sector, UINT count)
{
	spiFlashBufferRead(buff, ((u32)sector) << 12, ((u32)count) << 9);    
	return RES_OK; 
}

static int RAM_disk_status()
{
  return RES_OK;
}
static int RAM_disk_initialize()
{
  return RES_OK;
}
static int RAM_disk_write(const BYTE *buff, DWORD sector, UINT count)
{
  return RES_OK;
}
static int RAM_disk_read(BYTE *buff, DWORD sector, UINT count)
{
  return RES_OK;
}

static int USB_disk_status()
{
  return RES_OK;
}
static int USB_disk_initialize()
{
  return RES_OK;
}
static int USB_disk_write(const BYTE *buff, DWORD sector, UINT count)
{
  return RES_OK;
}
static int USB_disk_read(BYTE *buff, DWORD sector, UINT count)
{
  return RES_OK;
}