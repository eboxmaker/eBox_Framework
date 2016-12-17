/*
file   : *.cpp
author : link
version: V1.0
date   : 2016/12/14

Copyright 2016 link. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "USBKeyboard.h"

USBKeyboard key;

void setup()
{
    ebox_init();
}
int main(void)
{
    setup();
    while(1)
    {
			 key.printf("Hello World\r\n");
			delay_ms(1000);
    }

}


