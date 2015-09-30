/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"


void rtcsecit()
{
	uart1.printf("%02d:%02d:%02d\r\n",rtc.hour,rtc.min,rtc.sec);
}


void setup()
{
	ebox_init();
	uart1.begin(9600);
	rtc.begin();
	rtc.attach_interrupt(RTC_EVENT_SEC,rtcsecit);
	rtc.interrupt(RTC_EVENT_SEC,ENABLE);
}


int main(void)
{
	setup();

	while(1)
	{
		
		delay_ms(1000);
	}
}




