/*
file   : *.cpp
author : link
version: V1.0
date   : 2016/12/14

Copyright 2016 link. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "USBSerial.h"

USBSerial serial;

void setup()
{
    ebox_init();
}
int main(void)
{
    setup();
    while(1)
    {
			 serial.printf("I am a virtual serial port\n");
		   delay_ms(1000);
    }

}


