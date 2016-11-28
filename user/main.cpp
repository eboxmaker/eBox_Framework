/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "dht11.h"

Dht11 sensor(&PB2);

void setup()
{
    ebox_init();
    uart1.begin(115200);
    PB8.mode(OUTPUT_PP);

}
int main(void)
{
    setup();

    uart1.printf("DHT11 TEST PROGRAM \r\n");
    uart1.printf("LIBRARY VERSION: ");
    uart1.printf(Dht11::VERSION);
    uart1.printf("\r\n");
    while(1)
    {
        PB8.set();
        delay_us(50);
        PB8.reset();
        delay_us(50);
    }
}




