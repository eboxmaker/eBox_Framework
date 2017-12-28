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

SpiConfig_t config;

void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("\r\nuart1 115200 ok!");
    
    config.dev_num = 0;
    config.bit_order = SPI_FirstBit_MSB;
    config.mode = SPI_MODE0;
    config.prescaler = SPI_BaudRatePrescaler_32;
    
    spi1.begin(&config);


}
uint8_t buf[10];
int main(void)
{
    setup();

    while(1)
    {
        spi1.write(0X20);
        delay_ms(10);
        spi1.read(buf,10);
        uart1.write(buf,10);
        uart1.println();
        delay_ms(100);
    }


}




