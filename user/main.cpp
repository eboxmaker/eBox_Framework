/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "bsp_ebox.h"
#include "mmc_sd.h"
#include "w25xxx.h"
#include "interface/storage/filesystem/FileSystem.h"
#include "interface/storage/filesystem/FAT/FATFileSystem.h"

FATFileSystem fs("fs");

W25xxx flash(&PA15, &spi1);
SD sd(&PB12, &spi2);

void dirOpt()
{
//    res = f_mkdir("0:123"); //新建目录只能一级一级的建，即调用一次f_mkdir(),建一层目录而已，目录名不能以数字开头
//    if(res == FR_OK)
//        uart1.printf("\r\ncreat dir ok !");
//    else if(res == FR_EXIST)
//        uart1.printf("\r\ndir is exist !");
//    else
//        uart1.printf("\r\ncreate failed~~~~(>_<)~~~~");

//    res = f_opendir(&DirObject, "0:123"); //打开目录
//    if(res == FR_OK)
//    {
//        uart1.printf("\r\nopen dir ok !");
//        uart1.printf("\r\nclust  num：%d", DirObject.clust);
//        uart1.printf("\r\nsect num：%d", DirObject.sect);
//    }
//    else if(res == FR_NO_PATH)
//        uart1.printf("\r\ndir is not exist");
//    else
//        uart1.printf("\r\nopen dir failed~~~~(>_<)~~~~");
}
void setup()
{
    u8 ret;
    ebox_init();
    uart1.begin(115200);
    print_log();
    ret = fs.mount(&sd);

}
int main(void)
{
    setup();
    while(1)
    {

        delay_ms(1000);
    }


}




