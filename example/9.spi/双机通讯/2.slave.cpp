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
#include "spibridge.h"


void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("\r\nuart1 115200 ok!");
    PA2.mode(OUTPUT_PP);
    //    Dma1Ch3.attach(tc,DmaTcIrq);
    //    Dma1Ch2.attach(test,DmaTcIrq);
    begin();
    //

}
int main(void)
{
    setup();

    while(1)
    {
        loop();

        //        delay_ms(1000);
    }


}




