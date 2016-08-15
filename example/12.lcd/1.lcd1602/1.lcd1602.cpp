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
#include "lcd1602.h"

Lcd1602 lcd(&PB12, &PB13, &PB14, &PB15, &PB0, &PB1, &PB2, &PB3, &PB4, &PB5, &PB6, &PB7);

void setup()
{
    ebox_init();
    uart1.begin(9600);
    lcd.begin();

}

int8_t x;
int main(void)
{
    setup();
    while(1)
    {
        x++;
        lcd.printf(2, 0, "test = %05d", x);
        delay_ms(500);
    }


}




