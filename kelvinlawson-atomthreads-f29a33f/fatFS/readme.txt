//fatfs配置

_FS_MINIMIZE 3
_FS_READONLY 0
_USE_STRFUNC 0
_FS_RPATH    0
_USE_FIND    0
_USE_CHMOD   0
_USE_EXPAND  0
_USE_LABEL   0
_USE_MKFS    1
_USE_FORWARD 0
_MULTI_PARTITION 0

//可用函数
f_mount()
f_open()
f_close()
f_read()
f_write()
f_sync()
f_mkfs()
f_tell()
f_eof()
f_size()
f_error()
f_fdisk()
f_getlabel()
f_setlabel()

//不可用函数
f_lseek()
f_opendir()
f_close()
f_readdir()
f_findfirst()
f_findnext()
f_stat()
f_truncate()
f_unlink()
f_mkdir()
f_rename()
f_chdir()
f_chdrive()
f_getcwd()
f_chmod()
f_utime()
f_getlabel()			
f_setlabel()			
f_expand()				
f_forward()									
f_fdisk()					
f_putc()					
f_puts()					
f_printf()				
f_gets()	
f_getfree()
