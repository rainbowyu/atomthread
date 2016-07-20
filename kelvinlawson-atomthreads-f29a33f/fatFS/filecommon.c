#include "ff.h"
#include "stdio.h"
#include "stm8s.h"
void printFile(FIL *file);

void printFile(FIL *file)
{
  u8 readByte;
  UINT bw;
  f_lseek(file, 0);
  printf("\r\n");
  while (!(f_eof(file))){
    f_read(file, &readByte, 1, &bw);
    printf("%c",readByte);
  }
  printf("\r\n");  
  f_lseek(file, f_size(file));
}