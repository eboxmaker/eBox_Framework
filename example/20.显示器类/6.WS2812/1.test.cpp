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
#include "WS2812B.h"


WS2812B ws2812b(&PA11);

void setup()
{
    ebox_init();
    ws2812b.initialize(8);
}
int main(void)
{
    setup();
    ws2812b.set_color(10, 80, 0, 0);
    ws2812b.set_color(20, 70, 0, 1);
    ws2812b.set_color(30, 60, 0, 2);
    ws2812b.set_color(40, 50, 0, 3);
    ws2812b.set_color(50, 40, 0, 4);
    ws2812b.set_color(60, 30, 0, 5);
    ws2812b.set_color(70, 20, 0, 6);
    ws2812b.set_color(80, 10, 0, 7);
    ws2812b.enable();
    while(1)
    {
        delay_ms(1000);
    }
}


