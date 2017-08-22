/*
file   : *.cpp
author : link
version: V1.0
date   : 2016/12/14

Copyright 2016 link. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "usb_msd_sd.h"

USBMSD_SD usbmsd_sd(&PB12,&spi2);

void setup()
{
    ebox_init();
}
int main(void)
{
    setup();
    while(1)
    {
			delay_ms(1000);
    }

}


