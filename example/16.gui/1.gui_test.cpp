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



Button btn(&BtnPin,1);
Lcd lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
GUI gui;

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
    lcd.begin(1);
    btn.begin();
    
    gui.begin(&lcd,128,160);
    gui.fill_screen(BLACK);
    

}
void test(uint8_t totation)
{
    gui.set_rotation(totation);

    gui.fill_screen(BLACK);
    gui.set_color(WHITE);

    
    gui.set_font(&GUI_Font32_ASCII);
    gui.set_text_mode(LCD_DRAWMODE_NORMAL);
    gui.printf(0,0,"GUI TEST");
    
    gui.set_font(&GUI_Font24_ASCII);
    gui.set_text_mode(LCD_DRAWMODE_REV);
    gui.printf(0,32,"GUI TEST");
    
    gui.set_font(&GUI_Font16_ASCII);
    gui.set_text_mode(LCD_DRAWMODE_TRANS);
    gui.printf(0,56,"GUI TEST");
    
    gui.set_font(&GUI_FontHZ16X16);
    gui.set_text_mode(LCD_DRAWMODE_NORMAL);
    gui.printf(0,72,"强 TEST");
    
    gui.set_font(&GUI_Font8_ASCII);
    gui.set_text_mode(LCD_DRAWMODE_NORMAL);
    gui.printf(0,88,"GUI TEST");
    
    
    
    
    gui.set_font(&GUI_Font16_ASCII);
    gui.printf(10,110,"T:13");
    gui.set_font(&GUI_Font8_ASCII);
    gui.printf("o");
    gui.set_font(&GUI_Font16_ASCII);
    gui.printf(80,110,"H:24%%");


    gui.box(50,100,20,10,1);
    gui.box2(100,100,20,10,2);
    
    gui.btn_down(50,70,90,90);
    gui.btn_up(100,70,120,90);

}
int main(void)
{
    int i = 0;
    setup();
    test(i);

    while(1)
    {
        
        if(btn.click())
        {
            i++;
            i %=4;
            test(i);
        }
        btn.loop();
    }

}


