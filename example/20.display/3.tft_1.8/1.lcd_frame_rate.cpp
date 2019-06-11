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

uint32_t time_last;
u8 times;

//Lcd lcd(&PB13,&PB15,&PA13,&PA15,&PA14,&PA12);

void setup()
{
    ebox_init();
    PB8.mode(OUTPUT_PP);
    lcd.begin();
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


