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
        switch (sensor.read())
        {
        case Dht11::OK:
            uart1.printf("Humidity: %d\%% \r\n", sensor.getHumidity());

            uart1.printf("Temperature: %d'C \r\n ", sensor.getTemperature());
            break;

        case Dht11::ERROR_CHECKSUM:
            uart1.printf("Checksum error\r\n");
            break;

        case Dht11::ERROR_TIMEOUT:
            uart1.printf("Timeout error\r\n");
            break;

        default:
            uart1.printf("Unknown error\r\n");
            break;
        }

        delay_ms(1000);
    }
}




