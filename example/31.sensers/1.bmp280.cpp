/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2017/8/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "math.h"
#include "BMP280-3.3.h"


Adafruit_BMP280 bmp(&PA4,&spi1);


void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("\r\nuart1 115200 ok!\r\n");
    if (bmp.begin(1) == true)
    {  
        uart1.printf("find  BMP280 sensor!\r\n");
    }
    else
    {
        uart1.printf("Could not find a valid BMP280 sensor, check wiring!");
        while (1);

    }
        
}


int main(void)
{
    uint64_t start,end;

    setup();

    while(1)
    {
    uart1.print("Temperature = ");
    uart1.print(bmp.readTemperature());
    uart1.println(" *C");
    
    uart1.print("Pressure = ");
    uart1.print(bmp.readPressure());
    uart1.println(" Pa");

//    uart1.print("Approx altitude = ");
//    uart1.print(bmp.readAltitude(1013.25)); // this should be adjusted to your local forcase
//    uart1.println(" m");
    
    uart1.println();
    delay_ms(2000);


    }
}




