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

DS3231 ds(&si2c);

date_time_typedef t;
char time[9];
char date[9];

void setup()
{
	ebox_init();
	uart3.begin(9600);
	ds.begin(400000);
	
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
	ds.set_time(&t);
	while(1)
	{	
		ds.get_date_time(&t);
		ds.get_time(time);
		ds.get_date(date);
		uart3.printf("=======\r\n");
		uart3.printf("%02d-%02d-%02d %02d:%02d:%02d\r\n",t.year,t.month,t.date,t.hour,t.min,t.sec);

		uart3.printf(date);
		uart3.printf(" ");
		uart3.printf(time);
		uart3.printf("\r\n");
		delay_ms(1000);
	}


}




