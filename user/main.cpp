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

Lcd lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
GUI gui(&lcd,128,160);

void setup()
{
    ebox_init();
    PB8.mode(OUTPUT_PP);
    uart1.begin(115200);
    lcd.begin(1);
    
    
    gui.begin();
    gui.fill_screen(RED);
    
    uart1.printf("w:%d\n",gui.width());
    uart1.printf("h:%d\n",gui.height());
//    

//    
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

    gui.set_text_mode(LCD_TEXTMODE_TRANS);
    gui.set_font(&GUI_FontHZ16X16);
    gui.set_color(GREEN);
    gui.set_back_color(BLACK);
    gui.set_cursor(0,0);
    gui.disp_string("shen申t同强");
    
    gui.set_text_mode(LCD_TEXTMODE_NORMAL);
    gui.set_font(&GUI_Font32_ASCII);
    gui.set_color(GREEN);
    gui.set_back_color(BLACK);
    gui.set_cursor(0,16);
    gui.disp_string("shen申tQL同强");
    
    gui.set_font(&GUI_Font8_ASCII);
    gui.set_color(GREEN);
    gui.set_back_color(BLACK);
    gui.set_cursor(60,100);
    gui.disp_char('%');
    gui.disp_char('!');
    gui.disp_char('B');



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


