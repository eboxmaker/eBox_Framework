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
#include "w25x16.h"



W25x16 flash(&PA15, &spi1);


static FATFS fs;            // Work area (file system object) for logical drive
FATFS *fss;
DIR DirObject;       //目录结构
FIL fsrc;            // 文件结构

FRESULT res;

void fileOpt();

void dirOpt()
{
    res = f_mkdir("0:123"); //新建目录只能一级一级的建，即调用一次f_mkdir(),建一层目录而已，目录名不能以数字开头
    if(res == FR_OK)
        uart1.printf("\r\ncreat dir ok !");
    else if(res == FR_EXIST)
        uart1.printf("\r\ndir is exist !");
    else
        uart1.printf("\r\ncreate failed~~~~(>_<)~~~~");

    res = f_opendir(&DirObject, "0:123"); //打开目录
    if(res == FR_OK)
    {
        uart1.printf("\r\nopen dir ok !");
        uart1.printf("\r\nclust  num：%d", DirObject.clust);
        uart1.printf("\r\nsect num：%d", DirObject.sect);
    }
    else if(res == FR_NO_PATH)
        uart1.printf("\r\ndir is not exist");
    else
        uart1.printf("\r\nopen dir failed~~~~(>_<)~~~~");
    
    

        fileOpt();
}
void fileOpt()
{
    u8 ret;
    u8 buf[100];
    u8 readBuf[6] ;
    u32 bw = 0;
    u32 br = 0;

    for(int i = 0; i < 100; i++)
        buf[i] = '1';
    res = f_open(&fsrc, "0:125.txt", FA_WRITE | FA_READ | FA_CREATE_NEW ); //没有这个文件则创建该文件
    uart1.printf("\r\n");

    if(res == FR_OK)
    {
        
        uart1.printf("open/make file ok:%s\r\n",fsrc.dir_ptr);
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
        uart1.printf("file size：%d\r\n", fsrc.fsize);
        res = f_close(&fsrc);//关闭文件
        if(res == FR_OK)
        {
            uart1.printf("关闭文件成功 \r\n");
        }
        else
        {
            uart1.printf("关闭文件失败 err：%d\r\n", res);
        }
    }
    else if(res == FR_EXIST)
        uart1.printf("file exist\r\n");
    else
        uart1.printf("creat/open failed~~~~(>_<)~~~~ %d\r\n", res);


    /////////////////////////////////////////////////////////////////////////////////////
    u32 readsize;
    u32 buflen;
    buflen = sizeof(readBuf);
    res = f_open(&fsrc, "0:125.txt", FA_READ); //没有这个文件则创建该文件
    if(res == FR_OK)
    {
        uart1.printf("file size：%d\r\n", fsrc.fsize);
    }
    else
    {
        uart1.printf("open file err：%d\r\n", res);
    }
    readsize = 0;

    readsize = 0;
    do
    {
        res = f_read(&fsrc, readBuf, buflen, &br);
        if(res == FR_OK)
        {
            //			 uart1.printf("成功读取数据：%dBytes\r\n",br);
            //			 uart1.printf("data:%s\r\n",readBuf);
            uart1.write((const char *)readBuf, sizeof(readBuf));
        }
        else
        {
            uart1.printf("\r\nread failederr = %d\r\n",res);
        }
        readsize += buflen;
        f_lseek(&fsrc, readsize);

    }
    while(br == buflen);
    uart1.printf("\r\nread end\r\n");
    f_close(&fsrc);//关闭文件
//    f_mount(&fs, "0:", 0);





}

uint8_t rbuf[100];
uint8_t wbuf[100];
void setup()
{
    u8 ret;
    ebox_init();
    uart1.begin(115200);
    if(ret == 0)
        uart1.printf("sdcard init ok!\r\n");
    else
        uart1.printf("sdcard init failed;err = %d\r\n", ret);

    flash.begin();
    attach_sd_to_fat(0,&flash);

//    flash.erase_chip();
    res = f_mount(&fs, "0", 1);
    if(res == FR_OK)
        uart1.printf("mount ok!\r\n", res);
    else
    {
        uart1.printf("mount err!err = %d\r\n", res);

        res = f_mkfs("0",0,4096);
        if(res == FR_OK)
            uart1.printf("f_mkfs ok!\r\n", res);
        else
            uart1.printf("f_mkfs err!err = %d\r\n", res);
    }
    


}
u32 count;
int main(void)
{
    setup();
    fileOpt();
    dirOpt();
    while(1)
    {
    fileOpt();

        delay_ms(1000);
    }


}




