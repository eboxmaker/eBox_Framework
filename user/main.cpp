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
#include "ebox_encoder.h"
#include "oled_ssd1306_128x32.h"

//SoftI2c si2cx(&PB10, &PB11);
OledSSD1306_128x32 oled(&mcuI2c2);


void setup()
{
    ebox_init();
    uart1.begin(115200);
    oled.begin();
}
int x = 0;
int y = 0;
int main(void)
{

    setup();
    uint16_t temp;
    float speed;
	static uint64_t last_time = millis();
    static uint64_t last_time1 = millis();
    while(1)
    {
        last_time = millis();
        oled.clear();
        last_time1 = millis();
        uart1.printf("%d\r\n", last_time1 - last_time);

    unsigned char image[1024];

   oled.draw_line(0,0,10,20,1);
        
       

//        oled.draw_pixel(x,y,1);
        oled.fill_screen(1);
        oled.flush();
//        delay_ms(1000);
//        oled.show_num(0, 2, 25, 3, 16); //显示ASCII字符的码值
//        delay_ms(1000);
        x++;
        if(x>127) 
        {
            x= 0;
            y++;
            if(y>32) 
                y =0;

        }
        
//        oled.show_chinese(19, 1, 1); //中
        delay_ms(1);
    }
}




