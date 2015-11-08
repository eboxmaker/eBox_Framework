/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "button.h"


BUTTON btn(&PA0,1);

void setup()
{
	ebox_init();
	uart3.begin(9600);
	uart3.printf("\r\nok\r\n");
   btn.begin();

}



int main(void)
{
	setup();
	while(1)
	{
		btn.loop();
		if(btn.click())
		{
			uart3.printf("\r\nclick event!");
		}
		if(btn.release())
		{
			uart3.printf("\r\nrelease event!");
		}
		if(btn.pressed_for(2000,5))//长按两秒，执行5次
		{
			uart3.printf("\r\nlong press event!");
		}
	}
}




