#include "stdio.h"
#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_spi.h"
#include "atomtimer.h"
#include "atom.h"
#include "spiflash.h"
#include "spidriver.h"
#ifdef USESPIFFS
  #include "spiffs_config.h"
  #include "spiffs.h"
#endif

const u8 Tx_Buffer[] = "\n\r first fatFS test!!";

//common flash function 
void spiFlashTest(void);
u32  spiFlashReadID(void);
u8   flashEraseSector(u32 sector);
void flashWritePage(u8 *buf, u32 pageAddr, u16 writeNum);
u8   flashReadStateCom( void );
void flashWriteEnable( void );
void flashRead(u8 *buff, u32 addr, u32 readNum);
void spiFlashBufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void spiFlashWriteSector(u32 nSector, u8* pBuffer);
void spiFlashInit();

#ifdef SPIFFS
//spifFs function
void my_spi_read(int addr, int size, char *buf);
void my_spi_write(int addr, int size, char *buf);
void my_spi_erase(int addr, int size);
#endif

void spiFlashInit()
{
  GPIO_Init(FLASH_CS_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);  
  GPIO_Init(SPI_SCK_PIN,GPIO_MODE_OUT_PP_LOW_FAST);  
  GPIO_Init(SPI_MOSI_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);  
  GPIO_Init(SPI_MISO_PIN,GPIO_MODE_IN_PU_NO_IT);
  
  //打开flash电源 开发板独有
  GPIO_Init(GPIOE,GPIO_PIN_0,GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_WriteHigh(GPIOE,GPIO_PIN_0);
  
  //初始化SPI
  SPI_DeInit(); 
  SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_4, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x00);
  SPI_Cmd(ENABLE);
}

void spiFlashWriteSector(u32 nSector, u8* pBuffer)
{	
	u32 sectorAddr = nSector << 12;
	for(u16 i=0;i<FLASH_PAGES_PER_SECTOR;i++)
	{                 
		flashWritePage(pBuffer, sectorAddr, FLASH_PAGE_SIZE);
    sectorAddr += FLASH_PAGE_SIZE;  
		pBuffer    += FLASH_PAGE_SIZE;
	}
}

u8 flashEraseSector(u32 sector)
{
	u32  sectorAddr = ((u32)sector << 12);
  u8   timeout=TIMEOUT;
  flashWriteEnable();                                                           //开启写使能
  FILECS_LOW;                                                                   //使能设备

  spiSendByte(SECTOR_ERA_COM);                                                  //扇区擦除
  spiSendByte((u8)(sectorAddr>>16));                                            //发送地址
  spiSendByte((u8)(sectorAddr>>8));                                             //发送地址
  spiSendByte((u8)(sectorAddr));                                                //发送地址

  FILECS_HIGH;                                                                  //禁能设备
  while (flashReadStateCom()&STATUS_WIP){
    atomTimerDelay (1);
    if (0==(timeout--))break;
	}
  return 0x01;
}
/*
 * flash最多写一个page,256字节
 * *buf数据指针
 * pageAddr写入地址  (不一定是page头)
 * writeNum写入数据byte量
 */
void flashWritePage(u8 *buf, u32 pageAddr, u16 writeNum)
{
  u8 timeout=TIMEOUT;
  flashWriteEnable();
	FILECS_LOW;                                                                   //使能设备
	
	spiSendByte(PAGE_PRO_COM);                                                    //发送写数据命令
  spiSendByte((u8)(pageAddr>>16));                                              //发送地址
  spiSendByte((u8)(pageAddr>>8));                                               //发送地址
  spiSendByte((u8)(pageAddr));                                                  //发送地址
	
	for (u16 i=0; i<writeNum; i++){
		spiSendByte(*buf);																												  //数据发送
		buf++;
	}
	FILECS_HIGH;
  
  while (flashReadStateCom()&STATUS_WIP){
    atomTimerDelay (1);
    if (0==(timeout--))break;
	}
}

u8 flashReadStateCom( void )
{
  u8 status;
  FILECS_LOW;                                                                   //使能设备

  spiSendByte(READ_STA_COM);                                                    //读状态寄存器
  status = spiSendByte(0x00);                                                   //取得返回的数据

  FILECS_HIGH;                                                                  //禁能设备
  return status;
}

void flashWriteEnable( void )
{
	FILECS_LOW;                                                                   //使能设备
  spiSendByte(WRITE_EN_COM);                                                    //命令发送
  FILECS_HIGH;                                                                  //禁能设备
}

void spiFlashBufferRead(u8 *buff, u32 addr, u32 readNum)
{
  FILECS_LOW;                                                                   //使能设备

  spiSendByte(READ_DATA_COM);                                                   //发送读数据命令
  spiSendByte(addr>>16);                                                        //发送地址
  spiSendByte(addr>>8);                                                         //发送地址
  spiSendByte(addr);                                                 	          //发送地址
	for (u32 i=0; i<readNum; i++){
		*buff=spiSendByte(0x00);
		buff++;
	}
	
	FILECS_HIGH;   
}

u32 spiFlashReadID(void)
{
    u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    FILECS_LOW;

    spiSendByte(JEDEC_DEV_ID_COM);
    Temp0 = spiSendByte(0);
    Temp1 = spiSendByte(0);
    Temp2 = spiSendByte(0);

    FILECS_HIGH;

    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
    return Temp;
}

/**-----------------------------------------------------------------
  * SPI_FLASH_BufferWrite
  *         Writes block of data to the FLASH. In this function,
  *         the number of WRITE cycles are reduced,
  *         using Page WRITE sequence.
  *             pointer to the buffer  containing the data to be
  *             written to the FLASH.
  *         - WriteAddr : 
  *             FLASH's internal address to write to.
  *         - NumByteToWrite :
  *             number of bytes to write to the FLASH.
***----------------------------------------------------------------*/
void spiFlashBufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = WriteAddr % FLASH_PAGE_SIZE;
    count = FLASH_PAGE_SIZE - Addr;
    NumOfPage =  NumByteToWrite / FLASH_PAGE_SIZE;
    NumOfSingle = NumByteToWrite % FLASH_PAGE_SIZE;

    if (Addr == 0) /* WriteAddr is SPI_FLASH_PageSize aligned  */
    {
        if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
        {
          flashWritePage(pBuffer, WriteAddr, NumByteToWrite);
        }
        else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
          while (NumOfPage--)
          {
              flashWritePage(pBuffer, WriteAddr, FLASH_PAGE_SIZE);
              WriteAddr +=  FLASH_PAGE_SIZE;
              pBuffer += FLASH_PAGE_SIZE;
          }
          flashWritePage(pBuffer, WriteAddr, NumOfSingle);
        }
    }
    else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
    {
        if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
        {
            if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
            {
                temp = NumOfSingle - count;

                flashWritePage(pBuffer, WriteAddr, count);
                WriteAddr +=  count;
                pBuffer += count;

                flashWritePage(pBuffer, WriteAddr, temp);
            }
            else
            {
                flashWritePage(pBuffer, WriteAddr, NumByteToWrite);
            }
        }
        else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / FLASH_PAGE_SIZE;
            NumOfSingle = NumByteToWrite % FLASH_PAGE_SIZE;

            flashWritePage(pBuffer, WriteAddr, count);
            WriteAddr +=  count;
            pBuffer += count;

            while (NumOfPage--)
            {
                flashWritePage(pBuffer, WriteAddr, FLASH_PAGE_SIZE);
                WriteAddr +=  FLASH_PAGE_SIZE;
                pBuffer += FLASH_PAGE_SIZE;
            }

            if (NumOfSingle != 0)
            {
                flashWritePage(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}


void spiFlashTest(void)
{
	u32 FlashID = 0;
  u8  state[2] = {0};
  u8  Rx_Buffer[22];
  
  FlashID = spiFlashReadID();

  //printf("\r\n 芯片的ID是 0x%X", FlashID);

  if (FlashID == 0xEF4016)  
  {
    //擦除0x00扇区
    flashEraseSector(FLASH_SECTORTOERASE);
  
    printf("\r\n 写入的数据为: %s", Tx_Buffer);
    
    spiFlashBufferWrite((u8*)Tx_Buffer, FLASH_WRITEADDR, sizeof(Tx_Buffer));
  
    spiFlashBufferRead(Rx_Buffer, FLASH_READADDR, sizeof(Tx_Buffer));
  
    printf("\r\n 读出的数据为: %s", Rx_Buffer);
    
    for (u16 i=0;i< 22;i++)
    {
      if (*(Tx_Buffer+i)!=*(Rx_Buffer+i))
      {
        state[0] = 1;
      }
    }
    
    /* 擦除一个扇区 */    
    flashEraseSector(FLASH_SECTORTOERASE);
  
    /* 读取扇区数据 */
    spiFlashBufferRead(Rx_Buffer, FLASH_READADDR, sizeof(Rx_Buffer));
  
    /* 判断读取数据是否为0xff */
    for (u8 Index = 0; Index < 22; Index++)
    {
      if (Rx_Buffer[Index] != 0xFF)
      {
        state[1] = 1;
      }
    }
  
    if(0 == state[1])
    {
            printf("\r\n 读出数据都为 0xff\n\r");
    }
    else
    {
            printf("\r\n 读出数据不都为 0xff\n\r");
    }
  
    if(0 == (state[0]+state[1]))
    {
      printf("\r\n flash测试成功\n\r");
    }
    else
    {
      printf("\r\n flash测试失败!\n\r");
    }
  }
  else
  {
    printf("\r\n FLASH测试失败 \n\r");
  }
}
#ifdef USESPIFFS
  s32_t my_spi_read(u32_t addr, u32_t size, u8_t *buf)
  {
    return SPIFFS_OK;
  }
  s32_t my_spi_write(u32_t addr, u32_t size, u8_t *buf)
  {
    return SPIFFS_OK;
  }
  s32_t my_spi_erase(u32_t addr, u32_t size)
  {
    return SPIFFS_OK;
  }
#endif
