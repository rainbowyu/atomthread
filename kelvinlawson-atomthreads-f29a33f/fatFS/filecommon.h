#ifndef __FILECOMMON__
#define __FILECOMMON__
extern void printFile(FIL *file);
extern FRESULT fileFormat(BYTE * work,uint32_t worksize);
extern FRESULT scan_files (char* path);
#endif