#include "ff.h"
#include "stdio.h"
#include "stm8s.h"
void printFile(FIL *file);
FRESULT fileFormat(BYTE * work,uint32_t worksize);

void printFile(FIL *file)
{
  u8 readByte;
  UINT bw;
  int32_t a;
  f_lseek(file, 0);
  a=f_size(file);
  printf ("文件大小 %ld\r\n",a);
  while (!(f_eof(file))){
    f_read(file, &readByte, 1, &bw);
    printf("%c",readByte);
  }
  printf("\r\n");  
  f_lseek(file, 0);
}

//需要_MAX_SS内存
FRESULT fileFormat(BYTE * work,uint32_t worksize)
{
  
  FRESULT res;
  FATFS fs;           /* File system object */
  
  res=f_mount(&fs, "", 1);
  if (res != FR_OK)
	{
		printf("错误代码: %u\r\n",res);
	}	
  printf("开始格式化...\r\n");
  
  res = f_mkfs( "", FM_ANY|FM_SFD, 1024, work, worksize);
  if (res == FR_OK)
  {
    printf("格式化成功...\r\n");
  }
  else
  {
    printf("格式化失败...\r\n");
    printf("错误代码: %u\r\n",res);
  }
  f_mount(0, "", 1);
  
  return res;
}