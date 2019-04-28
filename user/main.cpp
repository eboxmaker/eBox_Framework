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


#include "t_menu.h"

//Oled4Spi oled(&PA4, &PA2, &PA1, &PA5, &PA7);
//FontLib font(&PA8,&PA5,&PA7,&PA6);

uint8_t buf[1024];
uint8_t buf2[256];
FontLib font(&PA3,&spi1);
bool api(uint16_t inner_code,uint8_t font_id,eBoxCharInfo_t *info)
{
    
    return font.get_data(inner_code,font_id,info);
}
void setup()
{
    ebox_init();
    uart1.begin(115200);
    oled.begin();
    font.begin();
    
    gui.begin(&oled,SSD1322_LCDWIDTH,SSD1322_LCDHEIGHT);
    gui.set_font_select(FONT_ALL_EXTERN);
    gui.set_font_hz_extern(Hz_32x32_ID);
    gui.set_font_ascii_extern(Ascii_16x32_ID);
    gui.attach(api );
    gui.fill_screen(0);
    gui.set_color(GUI_YELLOW);
    gui.set_font(&GUI_FontHZ16X16);
    gui.set_text_mode(TEXT_MODE_TRANS);
    gui.set_rotation(0);
    side_bar.max = 3;
    side_bar.prograss = 0;
    side_bar.orientation = 1;   
    
    
    print_log("Ñõ»¯ï¯²âÊÔÏîÄ¿",__DATE__);
    
    
    UART.setTimeout(10);
    LED1.mode(OUTPUT_PP);
    UART.printf("\r\n");
    displayCurrentMenu(&menu_demo);
}



int main(void)
{
    setup();

  



while(1)
    {



    menu_task(&menu_demo);

    }
}


