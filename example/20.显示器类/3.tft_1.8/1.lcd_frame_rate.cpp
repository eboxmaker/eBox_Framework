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

Lcd lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
uint32_t time_last;
u8 times;

void setup()
{
    ebox_init();
    PB8.mode(OUTPUT_PP);
    lcd.begin(1);
    lcd.clear(BLUE);
    uart1.begin(9600);
}
u16 color = RED;
int main(void)
{
    setup();
    while(1)
    {
        lcd.off();
        lcd.clear(RED);
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


