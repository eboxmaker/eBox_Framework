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
#include "lcd_1.8.h"
#include "at24c02.h"

//extern void attach_sd_to_fat(SD *sd);


static FATFS fs;            // Work area (file system object) for logical drive
FATFS *fss;
FRESULT res;
DIR DirObject;       //目录结构
DWORD free_clust;//空簇，空扇区大小


LCD lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
SD sd(&PB12, &spi2);
EEPROM ee(&i2c2);


u8 ret;
u8 buf[1024];

void getSDCardInfo()
{
    u32 rl;
    float x;
    ret = sd.get_CID(buf);
    uart1.printf("\r\n========================");
    uart1.printf("\r\nget CID Info,ret = %d", ret);
    uart1.printf("\r\n");
    uart1.printf((const char *)buf);

    rl = sd.get_capacity();
    uart1.printf("\r\n========================");
    uart1.printf("\r\n容量 = %d", rl / 1024 / 1024);


    res = f_getfree("/", &free_clust, &fss);
    if(res == FR_OK)
    {
        uart1.printf("\r\n该分区空闲扇区数为：%d", (fss->free_clust) * (fss->csize));
        uart1.printf("\r\n该分区大小为：%dM", (fss->free_clust) * (fss->csize) / 2048);
        uart1.printf("\r\n该分区空簇数为：%d", free_clust);
        uart1.printf("\r\n该分区空扇区数为：%d", free_clust * (fss->csize));
    }
    else
        uart1.printf("\r\n获取分区空簇失败,res = %d", res);
    uart1.printf("\r\n OVER !");


}
void setup()
{
    ebox_init();
    uart1.begin(9600);
    lcd.begin(1);
    ret = sd.begin(3);
    if(!ret)
        uart1.printf("\r\nsdcard init ok!");
    ee.begin(100000);
    
    lcd.clear(BLACK);

    attach_sd_to_fat(&sd);
    res = f_mount(&fs, "0:", 1);
    uart1.printf("\r\nres = %d", res);
    

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

int ee_check()
{

}

int w25x16_check()
{

}
int sd_check()
{

}

