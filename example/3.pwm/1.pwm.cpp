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

Pwm pwm1(&PB8);

//STM32 RUN IN eBox

void setup()
{
    ebox_init();
    uart1.begin(9600);
    pwm1.begin(1000, 20);
    pwm1.set_oc_polarity(1);
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

        pwm1.set_duty(y);
        delay_ms(10);
    }

}




