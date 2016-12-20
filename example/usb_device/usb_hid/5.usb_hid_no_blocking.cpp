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
//Two reports where will be stored values to send and received
HID_REPORT recv_report;
HID_REPORT send_report;

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
		btn.begin();
		led1.begin();
		led2.begin();
}
int main(void)
{
    setup();
    while(1)
    {
        //If a data is received, update led bus
        if (hid.readNB(&recv_report)) {
						set_led(recv_report.data[0]);
        }
 
        //if the bus of buttons has changed, send a report
        btn.loop();
        if(btn.click())
        {
						send_report.data[0] = 1;
						send_report.length = 64;
						hid.send(&send_report);
        }
        if(btn.release())
        {
						send_report.data[0] = 0;
						send_report.length = 64;
						hid.send(&send_report);
        }								        
    }
}


