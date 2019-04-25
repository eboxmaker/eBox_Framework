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

uint8_t buf[5120];
        FontLib font(&PA8,&spi1);
        FontLib::Frame_t frame;
void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("��ʼ");
    oled.begin();
    gui.begin(&oled,SSD1322_LCDWIDTH,SSD1322_LCDHEIGHT);
    int fsize = 16;
    font.begin();
    font.get_str_frame("ABCD",Ascii8x16,frame);
// while(1)
        for(int i = 0; i < frame.len; i++)
        {
            uart1.print(frame.ptr[i],HEX);
            uart1.print('\t');
        }
         uart1.println();
        
    for(int j = 0; j < 5; j++)
    {


        uint32_t addr = (('A' + j  - 0x20)<<4) + 0x1DD780;
        font.read_nbytes( addr ,buf,fsize);
        uart1.printf("====%c@0x%X====\r\n",'0' + j,addr);

        for(int i = 0; i < fsize; i++)
        {
            uart1.print(buf[i],HEX);
            uart1.print('\t');
        }
         uart1.println();
    }

while(1);
}

uint32_t last,now;
int x =0,y=0;

int main(void)
{
    setup();

    while(1)
    {
//        last = millis();
//        gui.fill_screen(GUI_BLACK);

//            
//        x = random(0,256);
//        y = random(0,64);
//        oled.draw_pixel(x,y,0xff);

//        oled.draw_circle(x,y,10,0xf);

//        gui.set_font(&GUI_FontHZ16X16);

//        gui.set_color(GUI_YELLOW);
//        gui.set_text_mode(TEXT_MODE_TRANS);
//        gui.printf(35, 33, "��ͬǿM2.5");
//        gui.printf(35, 50, "ug/m");

//        oled.flush();
//        now = millis();
//        
//        delay_ms(50);
//        uart1.printf("RUNNING:%d x = %d mem=%0.2fK\r\n",now - last,x,ebox_get_free()/1024.0);

    }
}


