#ifndef _SPIFLASH_
#define _SPIFLASH_

//spi pin
#define CS_PIN         GPIOG,GPIO_PIN_0
#define SPI_SCK_PIN  GPIOC,GPIO_PIN_5
#define SPI_MOSI_PIN GPIOC,GPIO_PIN_6
#define SPI_MISO_PIN GPIOC,GPIO_PIN_7

//Flash busy
#define STATUS_WIP   0x01

//CS pin
#define FILECS_HIGH     GPIO_WriteHigh(CS_PIN);
#define FILECS_LOW      GPIO_WriteLow(CS_PIN);

//Flash command
#define WRITE_EN_COM      0x06
#define SECTOR_ERA_COM    0x20
#define PAGE_PRO_COM      0x02
#define READ_STA_COM      0x05
#define READ_DATA_COM     0x03
#define JEDEC_DEV_ID_COM  0x9F

//FLASH inf
#define FLASH_PAGE_SIZE		      256
#define FLASH_SECTOR_SIZE	      512
#define FLASH_SECTOR_COUNT	    1024
#define FLASH_BLOCK_SIZE		    8192  		
#define FLASH_PAGES_PER_SECTOR	2

#define FLASH_SECTORTOERASE 0x00
#define FLASH_WRITEADDR     0x00
#define FLASH_READADDR      0x00

#define TIMEOUT 50

extern void spiFlashTest(void);
extern u32  spiFlashReadID(void);
extern u8   flashEraseSector(u32 sector);
extern void flashWritePage(u8 *buf, u32 pageAddr, u16 writeNum);
extern u8   flashReadStateCom( void );
extern void flashWriteEnable( void );
extern void spiFlashBufferRead(u8 *buff, u32 addr, u32 readNum);
extern void spiFlashBufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
extern void spiFlashWriteSector(u32 nSector, u8* pBuffer);
#endif