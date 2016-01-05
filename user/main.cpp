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
#include "color_convert.h"

COLOR_HSV hsv;
COLOR_RGB rgb;

LCD lcd(&PA13,&PA15,&PA11,&PA12,&spi2);

u8 index = 0x20;
u8 r;
u16 _color[3600];

void setup()
{
    ebox_init();
    PB8.mode(OUTPUT_PP);
    lcd.begin(1);
    lcd.clear(RED);
    uart1.begin(9600);
    
    lcd.column_order(1);
    lcd.row_order(1);

    lcd.front_color = RED;
    lcd.back_color = BLACK;
    hsv.s = 1;
    hsv.v = 0.5;
    hsv.h = 0;

    lcd.front_color = RED;
    if(index >= 0x50)index = 0x20;
    for(int i = 0; i < 160; i++){
        hsv.h = i*36/16;
        hsv.h %= 360;
        HSV_to_RGB(hsv,rgb);
        rgb_to_565(rgb,_color[i]);
       lcd.front_color = _color[i];
       lcd.draw_h_line(0,i,128);
    }
    lcd.disp_char8x16(0,0,index++);
    
    lcd.printf(2,2,"1231asddfgdsfgthkfhddddj2nhd");
    

    lcd.front_color = GREEN;
    lcd.draw_circle(50,50,50);
    lcd.draw_line(64,50,r,100);
    

}
int main(void)
{
	setup();
    u8 j;
	uint64_t highStart;
    uint64_t now;
	while(1)
	{
          highStart = micros();
		  delay_us(515555);
		  now= (micros() - highStart);
		  uart1.printf("%d,\r\n",now); 

	}

}


