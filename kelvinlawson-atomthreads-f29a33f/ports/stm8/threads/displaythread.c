#include "stdio.h"

#include "stm8s.h"
#include "u8g.h"
#include "u8g_stm8s.h"

#include "atom.h"
#include "atomtimer.h"
#include "atomsem.h"
#include "atomqueue.h"
#include "atommutex.h"

#include "displaythread.h"
ATOM_TCB display_tcb;
ATOM_SEM disCommondsem;

disComdata disCommandData;

void display_thread_func (uint32_t param);

void display_thread_func (uint32_t param)
{
  volatile uint32_t use=0,free=0;
  u8g_t u8g;
  
  u8g_InitComFn(&u8g,&u8g_dev_uc1701_mini12864_hw_spi,u8g_com_hw_spi_fn);
  u8g_SetFont(&u8g,u8g_font_04b_24);
  
  //500Byte
  volatile uint8_t displaybuff[8][50]={0};
  volatile uint8_t displayrow=0;
  
  while (1)
  {
    if (atomSemGet (&disCommondsem, 0) == ATOM_OK)
    {
      if (disCommandData.commandlist&NEWCOMMAND)
      { 
        displayrow++;
        for (uint8_t j=0;j<7;j++)
        {
          for (uint8_t i=0;i<50;i++)
          {
            displaybuff[7-j][i]=displaybuff[6-j][i];           
          }
        }
        for (uint8_t i=0;i<50;i++)
        {
          displaybuff[0][i]=disCommandData.buff[i];           
        }
      }
         
      u8g_FirstPage(&u8g);  
      do  
      {    
        for (uint8_t i =0;i<8 ;i++){
         u8g_DrawStr(&u8g, 0, 64-(i<<3), (char const*)displaybuff[i]);
        }
      } while ( u8g_NextPage(&u8g) );
      
      atomThreadStackCheck (&display_tcb, (uint32_t*)&use, (uint32_t*)&free);
      
      if (free<100)
        printf("display stack too low");
    }
  }
}