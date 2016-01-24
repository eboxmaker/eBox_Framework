/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox



#include "ebox.h"


uint32_t x;
uint32_t xx;
uint8_t flag1;
uint8_t flag;

TIM timer2(TIM2);
TIMERONE t1;

void t2it()
{
	xx++;
	if(xx == 1000)
	{
		flag = 1;
		xx = 0;
				PB9.write(!PB9.read());

	}
}
void t1it()
{
	x++;
	if(x == 1000)
	{
		flag1 = 1;
		x = 0;
				PB8.write(!PB8.read());

	}
}
void setup()
{
	ebox_init();
	uart1.begin(9600);
		PB8.mode(OUTPUT_PP);
		PB9.mode(OUTPUT_PP);

	timer2.begin(1000);
	timer2.attach_interrupt(t2it);
	timer2.interrupt(ENABLE);
	timer2.start();
	
	t1.begin(10000);
	t1.attach_interrupt(t1it);
	t1.interrupt(ENABLE);
	t1.start();
}


int main(void)
{
	setup();
	while(1)
	{
		if(flag == 1)
		{
			uart1.printf("\r\ntimer2 is triggered 1000 times !",xx);
			flag = 0;
		}
		if(flag1 == 1)
		{
			uart1.printf("\r\ntimer1 is triggered 1000 times !",xx);
			flag1 = 0;
		}
	}


}




