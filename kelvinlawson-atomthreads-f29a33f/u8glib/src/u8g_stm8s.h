#ifndef _U8G_STM8S_
#define _U8G_STM8S_
#include "stm8s_type.h"
#include "stm8s_gpio.h"
#include "stm8s_spi.h"
#define LCD_CD_PIN    GPIOE,GPIO_PIN_7
#define LCD_CS_PIN    GPIOE,GPIO_PIN_3
#define LCD_RST_PIN   GPIOE,GPIO_PIN_6
#define LCD_LIGHT_PIN GPIOA,GPIO_PIN_3

static void set_gpio_level(GPIO_TypeDef *PORT, GPIO_Pin_TypeDef Pin, uint8_t level) U8G_NOINLINE;
static uint8_t get_gpio_level(GPIO_TypeDef *PORT,GPIO_Pin_TypeDef Pin) U8G_NOINLINE;
static void spi_out(uint8_t data) U8G_NOINLINE;
static void spi_init(SPI_BaudRatePrescaler_TypeDef SPI_BaudRatePrescaler) U8G_NOINLINE;

static void u8g_10MicroDelay(void);
static void u8g_MicroDelay(void);
//static void u8g_Delay(uint32_t val);
static void delay_micro_seconds(uint32_t us);

extern uint8_t u8g_com_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
#endif