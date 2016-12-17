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

USBHID hid;
HID_REPORT recv;

void setup()
{
    ebox_init();
}
int main(void)
{
    setup();
    while(1)
    {
			hid.read(&recv);
    }

}


