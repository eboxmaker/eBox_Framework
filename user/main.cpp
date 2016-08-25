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
#include "bsp.h"




static FATFS fs;            // Work area (file system object) for logical drive
FATFS *fss;
DIR DirObject;       //目录结构
FIL fsrc;            // 文件结构

FRESULT res;


void fileOpt()
{
    uint8_t ret;
    uint8_t buf[100];
    uint8_t readBuf[6] ;
    uint32_t bw = 0;
    uint32_t br = 0;

   for(int i = 0; i < 100; i++)
        buf[i] = '1';
    res = f_open(&fsrc, "0:12345.txt", FA_WRITE | FA_READ | FA_CREATE_ALWAYS); //没有这个文件则创建该文件
    uart1.printf("\r\n");

    if(res == FR_OK)
    {
        uart1.printf("open/make file  O(∩_∩)O\r\n");
        uart1.printf("file flag:%d\r\n", fsrc.flag);
        uart1.printf("file size：%d\r\n", fsrc.fsize);
        uart1.printf("file ptr(start location)：%d\r\n", fsrc.fptr);
        //		uart1.printf("该文件开始簇号:%d\r\n",fsrc.org_clust);
        //		uart1.printf("该文件当前簇号：%d\r\n",fsrc.curr_clust);
        uart1.printf("dsect num:%d\r\n", fsrc.dsect);

        f_lseek(&fsrc, 0);
        do
        {
            res = f_write(&fsrc, buf, sizeof(buf), &bw);
            if(res)
            {
                uart1.printf("write error : %d\r\n", res);
                break;
            }
            uart1.printf("write ok!\r\n");
        }
        while (bw < sizeof(buf));  //  判断是否写完(bw > 100，表示写入完成)
    }
    else if(res == FR_EXIST)
        uart1.printf("file exist\r\n");
    else
        uart1.printf("creat/open failed~~~~(>_<)~~~~ %d\r\n", res);
    f_close(&fsrc);//关闭文件

    /////////////////////////////////////////////////////////////////////////////////////
    uint32_t readsize;
    uint32_t buflen;
    buflen = sizeof(readBuf);
    res = f_open(&fsrc, "0:12345.txt", FA_READ); //没有这个文件则创建该文件
    if(res == FR_OK)
    {
        uart1.printf("file size：%d\r\n", fsrc.fsize);
    }
    readsize = 0;
    do
    {
        res = f_read(&fsrc, readBuf, buflen, &br);
        if(res == FR_OK)
        {
            //			 uart1.printf("成功读取数据：%dBytes\r\n",br);
            //			 uart1.printf("data:%s\r\n",readBuf);
            uart1.printf_length((const char *)readBuf, sizeof(readBuf));
        }
        else
        {
            uart1.printf("\r\nread failed\r\n");
        }
        readsize += buflen;
        f_lseek(&fsrc, readsize);

    }
    while(br == buflen);
    uart1.printf("\r\nread end\r\n");
    f_close(&fsrc);//关闭文件
    f_mount(&fs, "0:", 0);




}
void setup()
{
    uint8_t ret;
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
uint32_t count;
int main(void)
{
    setup();
    fileOpt();
    while(1)
    {

        //uart1.printf("\r\nrunning！");
        delay_ms(1000);
    }


}




