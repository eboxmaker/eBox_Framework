/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */
 
 
#include "ebox.h"
#include "lcd_1.8.h"
#include "color_convert.h"
#include "gui.h"
COLOR_HSV hsv;
COLOR_RGB rgb;

Lcd lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
GUI gui(&lcd,128,160);

u8 index = 0x20;
u8 r;
u16 _color[3600];

void setup()
{
    uint16_t c;
    uint8_t d;
    ebox_init();
    PB8.mode(OUTPUT_PP);
    uart1.begin(115200);
    lcd.begin(1);
    
    
    gui.fill_screen(RED);
    gui.fill_rect(50,50,70,70);
    
    

    
    gui.set_color(BLUE);
    gui.draw_line(1,1,100,100);
    gui.draw_v_line(100,100,126);
    gui.draw_h_line(100,100,126);
    gui.draw_circle(100,100,25);
    gui.draw_circle(100,100,24);
    gui.draw_circle_helper(50,50,25,180);
    gui.set_color(YELLOW);
    gui.fill_circle(100,100,24);
    gui.fill_circle_helper(100,50,5,3,0);
    
    gui.draw_triangle(100,100,120,129,128,128);
    gui.fill_triangle(100,100,120,129,128,128);
    
    
    gui.draw_rect(0,0,60,30);
    gui.set_color(BLACK);
    gui.fill_rect(1,1,59,29);
    
    gui.draw_round_rect(0,50,100,40,10);
    gui.fill_round_rect(0,50,100,40,10);
//    lcd.column_order(1);
//    lcd.row_order(1);

//    lcd.front_color = RED;
//    lcd.back_color = BLACK;
//    hsv.s = 1;
//    hsv.v = 0.5;
//    hsv.h = 0;

//    lcd.front_color = RED;
//    if(index >= 0x50)index = 0x20;
//    for(int i = 0; i < 160; i++)
//    {
//        hsv.h = i * 36 / 16;
//        hsv.h %= 360;
//        HSV_to_RGB(hsv, rgb);
//        rgb_to_565(rgb, _color[i]);
//        lcd.front_color = _color[i];
//        lcd.draw_h_line(0, i, 128);
//    }
//    lcd.disp_char8x16(0, 0, index++);

//    lcd.printf(2, 2, "1231asddfgdsfgthkfhddddj2nhd");


//    lcd.front_color = GREEN;
//    lcd.draw_circle(50, 50, 50);
//    lcd.draw_line(64, 50, r, 100);


}
int main(void)
{
    setup();
    while(1)
    {
        delay_ms(1000);
        PB8.toggle();
    }

}


