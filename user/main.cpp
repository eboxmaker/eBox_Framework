/**
  ******************************************************************************
  * @file    pwm.cpp
  * @author  shentq
  * @version V2.0
  * @date    2016/08/14
  * @brief   ebox application example .
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/


#include "ebox.h"
#include "math.h"
#include "font.h"
#include "oled_ssd1322.h"
#include "gui.h"
#include "ebox_mem.h"
#include "gt30l32s4w.h"
#include "rtservice.h"





//Oled4Spi oled(&PA4, &PA2, &PA1, &PA5, &PA7);
OledSSD1322 oled(&PA4, &PA2, &PA1, &spi1);
GUI gui;
//FontLib font(&PA8,&PA5,&PA7,&PA6);

uint8_t buf[1024];
uint8_t buf2[256];
FontLib font(&PA3,&spi1);
bool api(uint16_t inner_code,uint8_t font_id,eBoxCharInfo_t *info)
{
    
    font.get_data(inner_code,font_id,info);
}

void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("开始");
    oled.begin();
    gui.begin(&oled,SSD1322_LCDWIDTH,SSD1322_LCDHEIGHT);
    font.begin();
}

uint32_t last,now;
int x =0,y=0;

int font_hz_id = Hz_12x12_ID;
int font_ascii_id = Ascii_6x12_ID;
int main(void)
{
    setup();
    gui.set_font_select(FONT_ALL_EXTERN);
    gui.attach(api );

    while(1)
    {
        gui.fill_screen(0);
//        font.get_one_hz_frame("：",Hz32x32,frame);
//        gui.drawBitmap(0,0,frame.ptr,frame.width,frame.hight,0xff,8);
//        
//        font.get_one_hz_frame("我",Hz32x32,frame);
//        gui.drawBitmap(15,0,frame.ptr,frame.width,frame.hight,0xff,8);

//       font.get_str_frame("123aaBD",Ascii8x16,frame);
//        gui.drawBitmap(48,0,frame.ptr,frame.width,frame.hight,0xff,8);
//        font.get_str_frame("你",Hz32x32,frame);
//        gui.drawBitmap(0,32,frame.ptr,frame.width,frame.hight,0xff,1);

        last = millis();
        x = random(0,256);
        y = random(0,64);
        oled.draw_pixel(x,y,0xff);

        oled.draw_circle(x,y,10,0xf);
        
        
        gui.set_color(GUI_YELLOW);
        gui.set_font(&GUI_FontHZ16X16);
        gui.set_text_mode(TEXT_MODE_TRANS);
//        gui.printf(35, 50, "ug/m");

//        oled.flush();
        font_hz_id++;
        font_ascii_id++;
        if(font_hz_id > Hz_32x32_ID){
            font_hz_id = Hz_12x12_ID;
            font_ascii_id = Ascii_6x12_ID;
        }
        gui.set_font_hz_extern(font_hz_id);
        gui.set_font_ascii_extern(font_ascii_id);
        
        gui.printf(0, 0, "12345ad");
        gui.printf(0, 32, "申同强ad我申");
        oled.flush();
        now = millis();
        
        delay_ms(2000);
        uart1.printf("RUNNING:%d x = %d mem=%0.2fK\r\n",now - last,x,ebox_get_free()/1024.0);

    }
}


