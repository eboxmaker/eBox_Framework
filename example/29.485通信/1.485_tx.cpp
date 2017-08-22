/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"

#include "lcd_1.8.h"

#define MAX485_T_R_PIN PA1

Lcd lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
uint8_t c = 'o';

void MAX485_T_MODE()
{
    MAX485_T_R_PIN.set();
}
void MAX485_R_MODE()
{
    MAX485_T_R_PIN.reset();
}

void setup()
{
    ebox_init();

    MAX485_T_R_PIN.mode(OUTPUT_PP);
    MAX485_T_MODE();
    uart1.begin(115200);

    PB8.mode(OUTPUT_PP);

    lcd.front_color = WHITE;
    lcd.begin(1);
    lcd.clear(RED);

}
int main(void)
{
    setup();
    uint8_t i = 0;
    while(1)
    {
        uart1.printf("%d", i);
        lcd.printf(0, 0, "send = %d", i);

        i++;
        delay_ms(1000);
    }

}


