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
#include "bsp_ebox.h"

#include "lcd_1.8.h"
#include "gui.h"

/**
	*	1	此例程演示了GUI
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"GUI example"
#define EXAMPLE_DATE	"2018-08-11"



Lcd lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
GUI gui;
Button btn(&BtnPin, 1);

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    lcd.begin(1);
    btn.begin();

    gui.begin(&lcd, 128, 160);
    gui.fill_screen(BLACK);


}
void test(uint8_t totation)
{
    gui.set_rotation(totation);

    gui.fill_screen(BLACK);
    gui.set_color(WHITE);


    gui.set_font(&GUI_Font16_ASCII);
    gui.set_text_mode(LCD_DRAWMODE_NORMAL);
    gui.printf(0, 0, "GUI TEST");


    //
    gui.set_color(RED);
    gui.fill_circle(50, 60, 40);
    gui.set_color(GREEN);
    gui.fill_circle(50, 60, 37);
    gui.set_color(BLACK);
    gui.set_text_mode(LCD_DRAWMODE_TRANS);
    gui.printf(35, 33, "PM2.5");
    gui.printf(35, 77, "ug/m");
    gui.set_font(&GUI_Font8_ASCII);
    gui.set_font(&GUI_Font32_ASCII);




    gui.set_color(RED);
    gui.fill_circle(115, 33, 25);
    gui.set_color(GREEN);
    gui.fill_circle(115, 33, 22);
    gui.set_color(BLACK);
    gui.set_font(&GUI_Font16_ASCII);
    gui.printf(98, 15, "CH2O");
    gui.set_font(&GUI_Font24_ASCII);
    gui.printf(103, 30, "12");




    gui.set_color(RED);
    gui.fill_circle(115, 83, 25);
    gui.set_color(GREEN);
    gui.fill_circle(115, 83, 22);
    gui.set_color(BLACK);
    gui.set_font(&GUI_Font16_ASCII);
    gui.printf(102, 65, "CO2");
    gui.set_font(&GUI_Font32_ASCII);
    gui.printf(35, 47, "95");
    gui.set_font(&GUI_Font24_ASCII);
    gui.printf(99, 80, "893");

    gui.set_color(GREEN);
    gui.fill_rect(150, 127, 160, 95);
    gui.set_color(YELLOW);
    gui.fill_rect(150, 95, 160, 63);
    gui.set_color(BLUE);
    gui.fill_rect(150, 63, 160, 31);
    gui.set_color(RED);
    gui.fill_rect(150, 31, 160, 0);

    gui.set_color(GREEN);
    gui.fill_triangle(140, 120, 150, 110, 140, 100);


}
int main(void)
{
    int i = 1;
    setup();
    test(i);

    while(1)
    {

        if(btn.click())
        {
            if(i == 1)
                i = 3;
            else
                i = 1;
            test(i);
        }
        btn.loop();
    }

}


