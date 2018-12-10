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
GUI gui;
uint8_t buf[] = {03, 03, 00, 00, 00, 01};
//
uint16_t tmp ;
void setup()
{
    ebox_init();
    PB8.mode(OUTPUT_PP);
    uart1.begin(115200);
    lcd.begin(1);


    gui.begin(&lcd, 128, 160);
    gui.fill_screen(BLACK);

    //
    //
    gui.set_color(WHITE);
    gui.set_font(&GUI_FontHZ16X16);
    gui.set_text_mode(LCD_TEXTMODE_REV);
    gui.set_rotation(0);
    gui.printf(50, 2, "…ÍPM2.5");
    gui.draw_h_line(10, 20, 50);
    gui.draw_v_line(50, 20, 30);
    gui.draw_line(10, 20, 50, 30);
    gui.draw_rect(12, 18, 48, 28);
    gui.fill_rect(12, 18, 48, 28);
    gui.draw_circle(12, 18, 10);

    delay_ms(2000);
    gui.set_rotation(1);

    gui.printf(50, 2, "…ÍPM2.5");
    gui.draw_h_line(10, 20, 50);
    gui.draw_v_line(50, 20, 30);
    gui.draw_line(10, 20, 50, 30);
    gui.draw_rect(12, 18, 48, 28);
    gui.fill_rect(12, 18, 48, 28);
    gui.draw_circle(12, 18, 10);


    delay_ms(2000);
    gui.set_rotation(2);
    gui.printf(50, 2, "…ÍPM2.5");
    gui.draw_h_line(10, 20, 50);
    gui.draw_v_line(50, 20, 30);
    gui.draw_line(10, 20, 50, 30);
    gui.draw_rect(12, 18, 48, 28);
    gui.fill_rect(12, 18, 48, 28);
    gui.draw_circle(12, 18, 10);

    delay_ms(2000);
    gui.set_rotation(3);
    gui.printf(50, 2, "…ÍPM2.5");
    gui.draw_h_line(10, 20, 50);
    gui.draw_v_line(50, 20, 30);
    gui.draw_line(10, 20, 50, 30);
    gui.draw_rect(12, 18, 48, 28);
    gui.fill_rect(12, 18, 48, 28);
    gui.draw_circle(12, 18, 10);

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


