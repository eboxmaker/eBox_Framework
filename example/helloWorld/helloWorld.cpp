/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"



USART uart1(USART1,&PA9,&PA10);
//USART uart2(USART3,&PA2,&PA3);
//USART uart3(USART3,&PB10,&PB11);

void setup()
{
	eBoxInit();
	uart1.begin(9600);
	
}


int main(void)
{
	setup();
	while(1)
	{
		uart1.printf("hello World !\r\n");
		delay_ms(1000);
	}


}




