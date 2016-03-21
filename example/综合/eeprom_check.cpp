/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "at24c02.h"

EEPROM ee(&i2c2);
uint8_t data;
void setup()
{
    ebox_init();
    uart1.begin(9600);
    ee.begin(400000);


    PA7.mode(AIN);
}

int16_t x, i;
uint8_t wbuf[512];
uint8_t rbuf[512];
int ret = 0;
int main(void)
{
    setup();


    while(1)
    {

        for(uint16_t i = 0; i < 256; i++)
        {
            wbuf[i] = random() % 256;
        }
        ee.write_byte(0, wbuf, 256);
        data = ee.read_byte(0, rbuf, 256);
        for(int i = 0; i < 256; i++)
        {
            if(wbuf[i] != rbuf[i])
            {
                ret = 1;
                break;
            }
        }
        if(ret == 1)
            uart1.printf("eeprom check ......[err]");
        else
            uart1.printf("eeprom check ......[OK]");
        delay_ms(10000);
    }


}




