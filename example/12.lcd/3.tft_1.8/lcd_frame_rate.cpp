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
        lcd.off();
     lcd.clear(RED);
        lcd.on();
     delay_ms(1000);
        lcd.off();
     lcd.clear(BLUE);
        lcd.on();
     delay_ms(1000);
        lcd.off();
     lcd.clear(GREEN);
        lcd.on();
     delay_ms(1000);
    PB8.toggle();
	}

}


