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
#include "button.h"


Button btn(&PA8, 1);

void setup()
{
    ebox_init();
    uart1.begin(9600);
    btn.begin();
    PB8.mode(OUTPUT_PP);
    PB9.mode(OUTPUT_PP);
    PB10.mode(OUTPUT_PP);
}



int main(void)
{
    setup();
    while(1)
    {
        btn.loop();
        if(btn.click())
        {
            PB8.write(!PB8.read());
            uart1.printf("\r\nclick event!");
        }
        if(btn.release())
        {
            PB9.write(!PB9.read());
            uart1.printf("\r\nrelease event!");
        }
        if(btn.pressed_for(2000, 5)) //长按两秒，执行5次
        {
            PB10.write(!PB10.read());
            uart1.printf("\r\nlong press event!");
        }
    }
}




