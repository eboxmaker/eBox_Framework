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
#include "font.h"
#include "oled_4spi.h"
#include "oled_i2c.h"


Oled4Spi oled(&PB3,&PB4,&PB5,&PB7,&PB6);

void setup()
{
    ebox_init();
    uart1.begin(115200);
    oled.begin();
    oled.show_string(0,0,"      eBox      ",16);
    oled.show_string(0,2,"bbs.eboxmaker.com",16);
    oled.show_string(0,6," 0.96 OLED TEST ",16);
}
int main(void)
{
    static uint64_t last_time = millis();
    static uint64_t last_time1 = millis();
    setup();
    uint16_t temp;
    float speed;
    while(1)
    {
        last_time = millis();
        oled.clear();
        last_time1 = millis();
        uart1.printf("time = %d\r\n",last_time1 - last_time);
        delay_ms(1000);
        oled.draw_bmp(0,0,128,8,(u8*)BMP1);
        delay_ms(1000);
		oled.show_num(103,6,25,3,16);//显示ASCII字符的码值 
        delay_ms(1000);
		oled.show_chinese(0,4,0);//中
        delay_ms(1000);
    }
}




