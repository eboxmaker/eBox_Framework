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

uint32_t time_last;
u8 times;

Lcd lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
uint32_t last1,last2;
void setup()
{
    ebox_init();
    PB8.mode(OUTPUT_PP);
    lcd.begin();
    lcd.clear(BLUE);
    uart1.begin(115200);
}
u16 color = RED;
int main(void)
{
    setup();
    while(1)
    {
        last1 = millis();
        lcd.off();
        lcd.clear(RED);
        last2 = millis();
        uart1.printf("time:%dms\r\n",last2 - last1);
        lcd.on();
        delay_ms(1000);
        lcd.off();
        lcd.clear(BLUE);
        lcd.on();
        delay_ms(1000);
        lcd.off();
        lcd.clear(GREEN);
        lcd.on();
        delay_ms(1000);
        PB8.toggle();
    }

}


