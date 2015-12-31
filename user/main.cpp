/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "lcd_1.8.h"

LCD lcd(&PA13,&PA15,&PA11,&PA12,&spi2);
uint32_t time_last;
u8 times;

void setup()
{
    ebox_init();
    PB8.mode(OUTPUT_PP);
    lcd.begin(1);
    lcd.clear(BLUE);
    uart1.begin(9600);
}
u16 color = RED;
int main(void)
{
	setup();
	while(1)
	{
       time_last = millis(); 
        times = 0;
    while(millis() - time_last < 1000)
    {
        lcd.clear(color++);
        times++;
    }       
    uart1.printf("times = %d\r\n",times);
    PB8.toggle();
	}

}


