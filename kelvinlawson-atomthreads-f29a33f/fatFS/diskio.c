/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include <stdio.h>
#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_spi.h"
#include "atomtimer.h"
#include "atom.h"
#include "spiflash.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0     */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2     */

/*private function*/
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
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat=RES_OK;
	int result=0;

	switch (pdrv) {
	case DEV_RAM :
		result = RAM_disk_status();

		// translate the reslut code here

		return stat;

	case DEV_MMC :
		result = MMC_disk_status();

		// translate the reslut code here

		return stat;

	case DEV_USB :
		result = USB_disk_status();

		// translate the reslut code here

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
	int result=0;

	switch (pdrv) {
	case DEV_RAM :
		result = RAM_disk_initialize();

		// translate the reslut code here

		return stat;

	case DEV_MMC :
		result = MMC_disk_initialize();

		// translate the reslut code here

		return stat;

	case DEV_USB :
		result = USB_disk_initialize();

		// translate the reslut code here

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
	int result=0;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		result = RAM_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

		result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

		result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

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
	int result=0;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		result = RAM_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

		result = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

		result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

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
	int result=0;

	switch (pdrv) {
	case DEV_RAM :
    switch (cmd)
    {
//      case CTRL_ERASE_SECTOR:
//        DWORD nFrom = *((DWORD*)buff);
//        DWORD nTo = *(((DWORD*)buff)+1);
//        for(DWORD i = nFrom;i <= nTo;i ++)
//          flashEraseSector(i);
//        break;
      
      case GET_BLOCK_SIZE:
        *(DWORD*)buff = 8192;
        break;
      
      case GET_SECTOR_SIZE:
        *(DWORD*)buff = 512;
        break;
      
      case GET_SECTOR_COUNT:
        *(DWORD*)buff = 1024;
        break;
        
        default:
          res = RES_PARERR;
        break;
    }
		// Process of the command for the RAM drive

		return res;

	case DEV_MMC :
    
		// Process of the command for the MMC/SD card

		return res;

	case DEV_USB :

		// Process of the command the USB drive

		return res;
	}

	return RES_PARERR;
}

static int MMC_disk_status()
{
  return 1;
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
  //初始化SPI IO口
  GPIO_Init(CS_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);  
  GPIO_Init(SPI_SCK_PIN,GPIO_MODE_OUT_PP_LOW_FAST);  
  GPIO_Init(SPI_MOSI_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);  
  GPIO_Init(SPI_MISO_PIN,GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOE,GPIO_PIN_0,GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_WriteHigh(GPIOE,GPIO_PIN_0);
  //初始化SPI
  SPI_DeInit(); 
  SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_4, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_HARD, 0x00);
  SPI_Cmd(ENABLE);
  return 1;
}

static int MMC_disk_write(const BYTE *buff, DWORD sector, UINT count)
{
	//u32 sectorAddr=0;
	//等待STATUS_WIP为0 busy判断
	//发送写使能命令
	//flashWriteEnable();
	BYTE *buf = (BYTE*)buff;       
	for(u16 i=0; i < count; i++){
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
  return 1;
}
static int RAM_disk_initialize()
{
  return 1;
}
static int RAM_disk_write(const BYTE *buff, DWORD sector, UINT count)
{
  return 1;
}
static int RAM_disk_read(BYTE *buff, DWORD sector, UINT count)
{
  return 1;
}

static int USB_disk_status()
{
  return 1;
}
static int USB_disk_initialize()
{
  return 1;
}
static int USB_disk_write(const BYTE *buff, DWORD sector, UINT count)
{
  return 1;
}
static int USB_disk_read(BYTE *buff, DWORD sector, UINT count)
{
  return 1;
}