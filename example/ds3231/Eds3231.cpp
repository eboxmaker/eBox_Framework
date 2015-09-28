/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "ds3231.h"

USART uart1(USART1,&PA9,&PA10);

DS3231 ds(&PA5,&PA4);

DateTime t;
char time[9];
char date[9];

void setup()
{
	eBoxInit();
	uart1.begin(9600);
	ds.begin(100000);
	
	t.year = 15;
	t.month = 7;
	t.date = 3;
	t.hour = 23;
	t.min = 59;
	t.sec = 55;
}
int main(void)
{
	setup();
	ds.setTime(&t);
	while(1)
	{	
		ds.getDateTime(&t);
		ds.getTime(time);
		ds.getDate(date);
		uart1.printf("=======\r\n");
		uart1.printf("%02d-%02d-%02d %02d:%02d:%02d\r\n",t.year,t.month,t.date,t.hour,t.min,t.sec);

		uart1.printf(date);
		uart1.printf(" ");
		uart1.printf(time);
		uart1.printf("\r\n");
		delay_ms(1000);
	}


}




