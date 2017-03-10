/*
file   : *.cpp
author : link
version: V1.0
date   : 2016/12/14

Copyright 2016 link. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "USBHID.h"
#include "bsp.h"
//We declare a USBHID device. By default input and output reports are 64 bytes long.
USBHID hid;

HID_REPORT recv;

void set_led(uint8_t data)
{
		if(data & 0x01)
		{
			led1.on();
		}
		else
		{
			led1.off();
		}
		if(data & 0x02)
		{
			led2.on();
		}
		else
		{
			led2.off();
		}
}

void setup()
{
    ebox_init();
		led1.begin();
		led2.begin();
}
int main(void)
{
    setup();
    while(1)
    {
			hid.read(&recv);
			set_led(recv.data[0]);
    }
}


