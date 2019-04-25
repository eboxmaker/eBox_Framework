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


//Oled4Spi oled(&PA4, &PA2, &PA1, &PA5, &PA7);
OledSSD1322 oled(&PA4, &PA2, &PA1, &spi1);
GUI gui;
//FontLib font(&PA8,&PA5,&PA7,&PA6);

uint8_t buf[1024];
uint8_t buf2[256];
        FontLib font(&PA8,&spi1);
        FontLib::Frame_t frame;
void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("开始");
    oled.begin();
    gui.begin(&oled,SSD1322_LCDWIDTH,SSD1322_LCDHEIGHT);
    font.begin();
//    buf2 = (uint8_t *)ebox_malloc(100);
//    gui.drawBitmap(0,0,frame.ptr,frame.width,frame.hight,0xff);
    oled.flush();
//while(1)
//        for(int i = 0; i < frame.len; i++)
//        {
//            uart1.print(frame.ptr[i],HEX);
//            uart1.print('\t');
//        }
//         uart1.println();
        
//    for(int j = 0; j < 5; j++)
//    {

//        uint32_t addr = (('A'  - 0x20)<<4) + 0x1DD780;
//        uart1.printf("====%c@0x%X====\r\n",'0' + j,addr);

//        font.read_nbytes( addr ,buf,Ascii8x16.nBytesOfChar);
//        font.get_char_buf('A',Ascii8x16,buf2);

//        uart1.print("1:");
//        for(int i = 0; i < Ascii8x16.nBytesOfChar; i++)
//        {
//            uart1.print(buf[i],HEX);
//            uart1.print('\t');
//        }
//        uart1.print("2:");
//        for(int i = 0; i < Ascii8x16.nBytesOfChar; i++)
//        {
//            uart1.print(buf2[i],HEX);
//            uart1.print('\t');
//        }
//         uart1.println();
//    }

//while(1);
}

uint32_t last,now;
int x =0,y=0;

int main(void)
{
    setup();

    while(1)
    {
        gui.fill_screen(0);
        font.get_one_hz_frame("：",Hz32x32,frame);
        gui.drawBitmap(0,0,frame.ptr,frame.width,frame.hight,0xff,8);
        
        font.get_one_hz_frame("我",Hz32x32,frame);
        gui.drawBitmap(15,0,frame.ptr,frame.width,frame.hight,0xff,8);

       font.get_str_frame("123aaBD",Ascii8x16,frame);
        gui.drawBitmap(48,0,frame.ptr,frame.width,frame.hight,0xff,8);
        font.get_str_frame("你",Hz32x32,frame);
        gui.drawBitmap(0,32,frame.ptr,frame.width,frame.hight,0xff,1);

        last = millis();
        x = random(0,256);
        y = random(0,64);
        oled.draw_pixel(x,y,0xff);

        oled.draw_circle(x,y,10,0xf);

        gui.set_font(&GUI_FontHZ16X16);

        gui.set_color(GUI_YELLOW);
        gui.set_text_mode(TEXT_MODE_TRANS);
        gui.printf(35, 33, "申同强M2.5");
        gui.printf(35, 50, "ug/m");

        oled.flush();
        now = millis();
        
        delay_ms(500);
        uart1.printf("RUNNING:%d x = %d mem=%0.2fK\r\n",now - last,x,ebox_get_free()/1024.0);

    }
}


