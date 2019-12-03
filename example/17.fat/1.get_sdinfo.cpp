/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "mmc_sd.h"
#include "wrapperdiskio.h"
#include "ff.h"


extern const char* err_str[];

static FATFS fs;            // Work area (file system object) for logical drive
FATFS *fss;
FRESULT res;
DIR DirObject;       //目录结构
DWORD free_clust;//空簇，空扇区大小


SD sd(&PB12, &spi2);

void getSDCardInfo()
{
    u8 ret;
    uint64_t rl;
    u8 buf[1024];

    ret = sd.get_CID(buf);
    uart1.printf("\r\n========================");
    uart1.printf("\r\nget CID Info,ret = %d", ret);
    uart1.printf("\r\n");
    uart1.printf((const char *)buf);

    rl = sd.get_capacity();
    uart1.printf("\r\n========================");
    uart1.printf("\r\ncapacity = %dMB", rl / 1024 / 1024);
    uart1.printf("\r\ncapacity = %0.1fGB", rl / 1024 / 1024 / 1024.0);

    uart1.printf("\r\nWaiting...");
    res = f_getfree("/", &free_clust, &fss);
    if(res == FR_OK)
    {
        uart1.printf("\r\npartition size：%dMB", (fss->free_clust) * (fss->csize) / 2048);
        uart1.printf("\r\npartition free sectors: %d", (fss->free_clust) * (fss->csize));
        uart1.printf("\r\npartition free clust:%d", free_clust);
        uart1.printf("\r\npartition free sector:%d", free_clust * (fss->csize));
    }
    else
        uart1.printf("\r\nget capacity faile,err = %d", res);
    uart1.printf("\r\nOVER !");


}
void setup()
{
    u8 ret;
    ebox_init();
    uart1.begin(115200);
    
    attach_sd_to_fat(0,&sd);
    res = f_mount(&fs, "0", 1);
    if(res == FR_OK)
        uart1.printf("mount ok!\r\n", res);
    else
        uart1.printf("err code:%s\r\n", err_str[res]);

}
u32 count;
int main(void)
{
    setup();
    getSDCardInfo();
    while(1)
    {

        delay_ms(1000);
    }


}



const char* err_str[] = 
{
    "FR_OK",				/* (0) Succeeded */
    "FR_DISK_ERR",			/* (1) A hard error occurred in the low level disk I/O layer */
    "FR_INT_ERR",				/* (2) Assertion failed */
    "FR_NOT_READY",			/* (3) The physical drive cannot work */
    "FR_NO_FILE",				/* (4) Could not find the file */
    "FR_NO_PATH",				/* (5) Could not find the path */
    "FR_INVALID_NAME",		/* (6) The path name format is invalid */
    "FR_DENIED",				/* (7) Access denied due to prohibited access or directory full */
    "FR_EXIST",				/* (8) Access denied due to prohibited access */
    "FR_INVALID_OBJECT",		/* (9) The file/directory object is invalid */
    "FR_WRITE_PROTECTED",		/* (10) The physical drive is write protected */
    "FR_INVALID_DRIVE",		/* (11) The logical drive number is invalid */
    "FR_NOT_ENABLED",			/* (12) The volume has no work area */
    "FR_NO_FILESYSTEM",		/* (13) There is no valid FAT volume */
    "FR_MKFS_ABORTED",		/* (14) The f_mkfs() aborted due to any parameter error */
    "FR_TIMEOUT",				/* (15) Could not get a grant to access the volume within defined period */
    "FR_LOCKED",				/* (16) The operation is rejected according to the file sharing policy */
    "FR_NOT_ENOUGH_CORE",		/* (17) LFN working buffer could not be allocated */
    "FR_TOO_MANY_OPEN_FILES",	/* (18) Number of open files > _FS_LOCK */
    "FR_INVALID_PARAMETER"	/* (19) Given parameter is invalid */
};


