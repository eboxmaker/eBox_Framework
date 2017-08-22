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
    uart1.printf("\r\ncapacity = %0.1fGB", rl / 1024 / 1024/1024.0);

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
    ret = sd.begin(3);
    if(ret == 0)
        uart1.printf("sdcard init ok!\r\n");
    else
        uart1.printf("sdcard init failed;err = %d\r\n",ret);
        
    attach_sd_to_fat(&sd);

    res = f_mount(&fs, "0", 1);
    if(res == FR_OK)
        uart1.printf("mount ok!\r\n", res);
    else
        uart1.printf("mount err!err = %d\r\n", res);

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




