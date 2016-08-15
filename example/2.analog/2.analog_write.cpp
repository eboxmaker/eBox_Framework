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
#include "math.h"


void setup()
{
    ebox_init();

}

float x;
uint16_t y;
int main(void)
{

    setup();

    while(1)
    {
        x = x + PI * 0.01;
        if(x >= PI)x = 0;
        y = 2000 - (sin(x) + 1) * 1000;

        analog_write(&PB8, y);
        delay_ms(10);
    }


}