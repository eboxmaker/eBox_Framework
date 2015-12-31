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
    lcd.clear(BLACK);
    lcd.column_order(1);
    lcd.row_order(1);
    lcd.front_color = BLUE;
    lcd.draw_h_line(10,10,100);
    lcd.draw_v_line(10,10,100);
    lcd.fill_rect(11,11,101,101);
    lcd.front_color = RED;
    lcd.back_color = BLACK;
    uart1.begin(9600);
}
u8 index = 0x20;
u8 r;
int main(void)
{
	setup();
	while(1)
	{
        lcd.front_color = RED;
        if(index >= 0x50)index = 0x20;
        lcd.disp_char8x16(1,1,index++);
        
        lcd.printf(2,2,"1231asddfgdsfgthkfhddddj2nhd");
        lcd.front_color = GREEN;
        lcd.draw_circle(50,50,50);
        lcd.draw_line(64,50,r,100);
        r++;
        r %= 128;
        
        delay_ms(100);
    uart1.printf("times = %d\r\n",times);
    PB8.toggle();
	}

}


