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

#include "bsp_ebox.h"
#include "ebox.h"
#include "math.h"
#include "font.h"
#include "ebox_mem.h"
#include "gt30l32s4w.h"
#include "rtservice.h"


#include "..\component\menu\t_menu example\t_menu.h"



//Oled4Spi oled(&PA4, &PA2, &PA1, &PA5, &PA7);
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
    oled.begin();
    font.begin();
    
    gui.begin(&oled,SSD1322_LCDWIDTH,SSD1322_LCDHEIGHT);
    gui.set_font_select(FONT_ALL_EXTERN);
    gui.set_font_hz_extern(Hz_24x24_ID);
    gui.set_font_ascii_extern(Ascii_12x24_ID);
    gui.attach(api );
    gui.fill_screen(0);
    gui.set_color(GUI_YELLOW);
    gui.set_font(&GUI_FontHZ16X16);
    gui.set_text_mode(TEXT_MODE_TRANS);
    
    
    	print_log("test",__DATE__);
		UART.setTimeout(200);
		LED1.mode(OUTPUT_PP);
		UART.printf("\r\n");
	//	displayCurrentMenu(&s_tMenuDemo);
}



int main(void)
{
    setup();

  
//    for(int i = 0; i < 10; i++)
//        uart1.println(test[i]);  
    
 GuiList list(10,10,50,10);
            list.max = 5;

              list.prograss = 0;
         list.orientation = 0;
//       gui.draw_rect(128,30,200,62);
//       oled.flush();
//        delay_ms(1000);
    int i = 0;
while(1)
    {
 gui.set_rotation(i);
//       gui.printf(50,0,"ï¯³ØÉè¶¨ÎÂ¶È");
//        gui.set_font_hz_extern(Hz_24x24_ID);
//        gui.printf(180,34,"700¡æ");
//        gui.draw_circle(32,32,10);
        if(list.prograss >= list.max)
        {
            list.prograss = 0;
            gui.clear();
        }
//        gui.fill_rect(0,0,128,32);
//        oled.flush();
//    delay_ms(1000);
//        gui.fill_rect(1,1,127,31,0);
//        //gui.fill_rect(0,0,32,10);
        gui.drawList(list);
//        oled.flush();
//   // menu_task(&s_tMenuDemo);
//    delay_ms(1000);
//        oled.draw_pixel(9,9,0xff);
//        gui.draw_h_line(11,11,13);
//        gui.draw_v_line(11,11,13);
//        
////        gui.draw_pixel(20,20,0xff);
////        gui.draw_h_line(21,21,3);
////        gui.draw_v_line(22,22,3);     
////        
//        gui.draw_rect(26,22,30,30);



//i++;
//       oled.flush();
//if(i == 4)
//{
//    i = 0;
//        delay_ms(1000);
//    gui.clear();
       oled.flush();
//}//        gui.drawList(list);
         list.prograss++;
        delay_ms(1000);

    }
}


