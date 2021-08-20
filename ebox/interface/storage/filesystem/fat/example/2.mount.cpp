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
#include "interface/storage/filesystem/FileSystem.h"
#include "interface/storage/filesystem/FAT/FATFileSystem.h"

FATFileSystem fs("fs");

SD sd(&PB12, &spi2);

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




