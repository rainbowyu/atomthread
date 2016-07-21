#include "stdio.h"

#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_spi.h"
#include "atomtimer.h"
#include "atom.h"

#include "u8g.h"
#include "u8g_stm8s.h"

#include "spidriver.h"
#include "stm8s_spi.h"
#include "stm8s_gpio.h"

static void set_gpio_level(GPIO_TypeDef *PORT,GPIO_Pin_TypeDef Pin, uint8_t level) ;
static uint8_t get_gpio_level(GPIO_TypeDef *PORT,GPIO_Pin_TypeDef Pin) ;
static void spi_out(uint8_t data) ;
static void spi_init(SPI_BaudRatePrescaler_TypeDef SPI_BaudRatePrescaler) ;

static void u8g_10MicroDelay(void);
static void u8g_MicroDelay(void);
static void delay_micro_seconds(uint32_t us);

uint8_t u8g_com_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);


static void set_gpio_level(GPIO_TypeDef *PORT, GPIO_Pin_TypeDef Pin, uint8_t level)
{
  if(level != 0){
		GPIO_WriteHigh(PORT, Pin);
	}else{
		GPIO_WriteLow(PORT, Pin);
	}
}

static uint8_t get_gpio_level(GPIO_TypeDef *PORT, GPIO_Pin_TypeDef Pin)
{
   return GPIO_ReadInputPin(PORT, Pin);
}

//SpeedSet:
//SPI_BAUDRATEPRESCALER_2    
//SPI_BAUDRATEPRESCALER_4   
//SPI_BAUDRATEPRESCALER_8    
//SPI_BAUDRATEPRESCALER_16 
static void spi_init(SPI_BaudRatePrescaler_TypeDef SPI_BaudRatePrescaler)
{
  GPIO_Init(LCD_CS_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(LCD_CD_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(LCD_RST_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);  
  GPIO_Init(LCD_LIGHT_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);
  
  GPIO_Init(SPI_SCK_PIN,GPIO_MODE_OUT_PP_LOW_FAST);  
  GPIO_Init(SPI_MOSI_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);  
  GPIO_Init(SPI_MISO_PIN,GPIO_MODE_IN_PU_NO_IT);
  
  //打开flash电源 开发板独有
  GPIO_Init(GPIOE,GPIO_PIN_0,GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_WriteHigh(GPIOE,GPIO_PIN_0);
  
  //打开背光
  GPIO_WriteLow(LCD_LIGHT_PIN);
  
  //关闭FLASH CS
  GPIO_WriteLow(GPIOG,GPIO_PIN_0);
  
  //初始化SPI
  SPI_DeInit(); 
  SPI_Init(SPI_FIRSTBIT_MSB, SPI_BaudRatePrescaler, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x00);
  SPI_Cmd(ENABLE);
}

static void spi_out(uint8_t data)
{
  spiSendByte( data );
}

static void delay_micro_seconds(uint32_t us)
{
  for (uint32_t i=0;i<us;i++)
  {
    asm ("nop");asm ("nop");
    asm ("nop");asm ("nop");
    asm ("nop");asm ("nop");
    asm ("nop");asm ("nop");
    asm ("nop");asm ("nop");
    asm ("nop");asm ("nop");
  }
}

static void u8g_MicroDelay(void)
{
  delay_micro_seconds(1);
}

static void u8g_10MicroDelay(void)
{
  delay_micro_seconds(10);
}

/*========================================================================*/
/* u8glib com procedure */

uint8_t u8g_com_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_STOP:
      break;
    
    case U8G_COM_MSG_INIT:

       if ( arg_val <= U8G_SPI_CLK_CYCLE_50NS )
      {
        spi_init(SPI_BAUDRATEPRESCALER_2);
      }
      else if ( arg_val <= U8G_SPI_CLK_CYCLE_300NS )
      {
        spi_init(SPI_BAUDRATEPRESCALER_4);
      }
      else if ( arg_val <= U8G_SPI_CLK_CYCLE_400NS )
      {
        spi_init(SPI_BAUDRATEPRESCALER_4);
      }
      else
      {
        spi_init(SPI_BAUDRATEPRESCALER_8);
      }
      u8g_MicroDelay();      
      break;
    
    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      u8g_10MicroDelay();
      set_gpio_level(LCD_CD_PIN, arg_val);
      u8g_10MicroDelay();
     break;

    case U8G_COM_MSG_CHIP_SELECT:
      if ( arg_val == 0 )
      {
        /* disable */
        uint8_t i;
        /* this delay is required to avoid that the display is switched off too early --> DOGS102 with LPC1114 */
        for( i = 0; i < 5; i++ )u8g_10MicroDelay();
        set_gpio_level(LCD_CS_PIN, 1);
      }
      else
      {
        /* enable */
        set_gpio_level(LCD_CS_PIN, 0);
      }
      u8g_MicroDelay();
      break;
      
    case U8G_COM_MSG_RESET:
      set_gpio_level(LCD_RST_PIN, arg_val);
      u8g_10MicroDelay();
      break;
      
    case U8G_COM_MSG_WRITE_BYTE:
      spi_out(arg_val);
      u8g_MicroDelay();
      break;
    
    case U8G_COM_MSG_WRITE_SEQ:
    case U8G_COM_MSG_WRITE_SEQ_P:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          spi_out(*ptr++);
          arg_val--;
        }
      }
      break;
  }
  return 1;
}

