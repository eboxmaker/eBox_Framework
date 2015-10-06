/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "l3g4200d.h"

L3G4200D lg(&si2c2);
void setup()
{
	ebox_init();
	uart1.begin(9600);
	uart1.printf("buadrate = 9600");
	lg.begin(100000);
   
}
u32 count;
int main(void)
{
	setup();
	while(1)
	{
		lg.read();
		uart1.printf("==========================\r\n");
		lg.test();
		uart1.printf("x:%0.2f;\r\n",lg.g.x);
		uart1.printf("y:%0.2f;\r\n",lg.g.y);
		uart1.printf("z:%0.2f;\r\n",lg.g.z);
		delay_ms(1000);
	}


}




