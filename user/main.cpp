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
#include "crc16.h"
Lcd lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
GUI gui(&lcd,128,160);
uint8_t buf[]={03,03,00,00,00,01};
//    
    uint16_t tmp ;
void setup()
{
    ebox_init();
    PB8.mode(OUTPUT_PP);
    uart1.begin(115200);
    lcd.begin(1);
    
    
    gui.begin();
    gui.fill_screen(BLACK);
    
    uart1.printf("w:%d\n",gui.width());
    uart1.printf("h:%d\n",gui.height());
//    
     tmp = crc16(buf,6);
    uart1.printf("crc:%x\n",tmp);
//    
    gui.set_color(WHITE);
    gui.set_font(&GUI_Font16_ASCII);
    gui.printf(5,2,"PM2.5");
    gui.printf(10,110,"TEMP:13");
    gui.set_font(&GUI_Font8_ASCII);
    gui.printf("o");
    gui.printf(80,110,"humidity:24%");
    
    //
    gui.set_color(RED);
    gui.draw_circle(50,60,40);
    gui.draw_circle(50,60,39);
    gui.draw_circle(50,60,38);
    gui.set_color(GREEN);
    gui.fill_circle(60,50,37);
    gui.set_color(BLACK);
    gui.set_text_mode(LCD_DRAWMODE_TRANS);
    gui.printf(35,33,"PM2.5");
    gui.printf(35,77,"ug/m");
    gui.set_font(&GUI_Font8_ASCII);
    gui.printf("3");
    gui.set_font(&GUI_Font32_ASCII);
    gui.printf(35,47,"95");




    gui.set_color(RED);
    gui.draw_circle(115,33,25);
    gui.draw_circle(115,33,24);
    gui.draw_circle(115,33,23);
    gui.set_color(GREEN);
    gui.fill_circle(33,115,22);
    gui.set_color(BLACK);
    gui.set_font(&GUI_Font16_ASCII);
    gui.printf(98,15,"CH2O");
    gui.set_font(&GUI_Font24_ASCII);
    gui.printf(103,30,"12");






    gui.set_color(RED);
    gui.draw_circle(115,83,25);
    gui.draw_circle(115,83,24);
    gui.draw_circle(115,83,23);
    gui.set_color(GREEN);
    gui.fill_circle(83,115,22);
    gui.set_color(BLACK);
    gui.set_font(&GUI_Font16_ASCII);
    gui.printf(102,65,"CO2");
    gui.set_font(&GUI_Font32_ASCII);
    gui.printf(35,47,"95");
    gui.set_font(&GUI_Font24_ASCII);
    gui.printf(99,80,"893");
    
    gui.set_color(GREEN);
    gui.fill_rect(0,150,30,155);
    gui.set_color(YELLOW);
    gui.fill_rect(30,150,60,155);
    gui.set_color(BLUE);
    gui.fill_rect(60,150,90,155);
    gui.set_color(RED);
    gui.fill_rect(90,150,120,155);
    
    
    gui.set_color(GREEN);
    gui.fill_triangle(10,143,15,150,20,143);

//    gui.set_color(BLUE);
//    gui.draw_line(1,1,100,100);
//    gui.draw_v_line(100,100,126);
//    gui.draw_h_line(100,100,126);
//    gui.draw_circle(100,100,25);
//    gui.draw_circle(100,100,24);
//    gui.draw_circle_helper(50,50,25,180);
//    gui.set_color(YELLOW);
//    gui.fill_circle(100,100,24);
//    gui.fill_circle_helper(100,50,5,3,0);
//    
//    gui.draw_triangle(100,100,120,129,128,128);
//    gui.fill_triangle(100,100,120,129,128,128);
//    
//    
//    gui.draw_rect(0,0,60,30);
//    gui.set_color(BLACK);
//    gui.fill_rect(1,1,59,29);
//    
//    gui.draw_round_rect(0,50,100,40,10);
//    gui.fill_round_rect(0,50,100,40,10);

//    gui.set_text_mode(LCD_TEXTMODE_TRANS);
//    gui.set_font(&GUI_FontHZ16X16);
//    gui.set_color(GREEN);
//    gui.set_back_color(BLACK);
//    gui.set_cursor(0,0);
//    gui.disp_string("she1n…ÍtÕ¨«ø");
//    gui.printf("\nc…Íccpu:%d",cpu.clock.core);
//    
//    gui.set_text_mode(LCD_TEXTMODE_NORMAL);
//    gui.set_font(&GUI_Font32_ASCII);
//    gui.set_color(GREEN);
//    gui.set_back_color(BLACK);
//    gui.disp_string("shentQL");
//    gui.printf("asdf\nd%d",2);
//    
//    gui.set_font(&GUI_Font8_ASCII);
//    gui.set_color(GREEN);
//    gui.set_back_color(BLACK);
//    gui.set_cursor(60,100);
//    gui.disp_char('%');
//    gui.disp_char('!');
//    gui.disp_char('B');



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


