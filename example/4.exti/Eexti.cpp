/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"


uint32_t xx;

EXTIx ex(&PA8,EXTI_Trigger_Falling);

void exit()
{
	xx++;
	uart1.printf("\r\nxx = %d",xx);
}
void setup()
{
	ebox_init();
	uart1.begin(9600);
	ex.begin();
	ex.attachInterrupt(exit);
	ex.interrupt(ENABLE);
}


int main(void)
{
	setup();
	while(1)
	{
		;
	}


}




