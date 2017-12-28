/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "w5500.h"
#include "ebox_slave_spi.h"

SlaveSpi slave_spi1(SPI1, &PA5, &PA6, &PA7);


void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("\r\nuart1 115200 ok!");
    slave_spi1.begin();


}
int main(void)
{
    setup();

    while(1)
    {
        delay_ms(1000);
    }


}




