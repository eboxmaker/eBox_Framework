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

//Oled4Spi oled(&PA4, &PA2, &PA1, &PA5, &PA7);
Oled4Spi oled(&PA4, &PA2, &PA1, &spi1);
extern uint8_t BMP3[];
void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("¿ªÊ¼");
    oled.begin();


}
#include "font.h"
    uint32_t last,now;

int main(void)
{
    setup();

    int x =0,i = 0;
                oled.clear();
    uart1.printf("Çå¿Õ\r\n");
    while(1)
    {
            last = millis();
        //oled.draw_bmp(BMP3,1,0x04);
//        delay_ms(100);
            if(i == 0)
            {
                x++;
                
               if(x == 0xf)
                   i = 1;
            }
            else
            {
               x--;
                if(x == 0)
                    i = 0;
            }
           oled.clear(x);
//            oled.clear(0x08);
//            oled.clear(0x015);
            now = millis();
            delay_ms(500);
        uart1.printf("RUNNING:%d x = %d\r\n",now - last,x);

    }
}


