/**
  ******************************************************************************
  * @file    pwm.cpp
  * @author  shentq
  * @version V1.2
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

Pwm pwm1(&PA0);
Pwm pwm2(&PA1);

void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("core:%d\r\n",cpu.clock.core);
    uart1.printf("hclk:%d\r\n",cpu.clock.hclk);
    uart1.printf("pclk1:%d\r\n",cpu.clock.pclk1);
    uart1.printf("pclk2:%d\r\n",cpu.clock.pclk2);
    
    pwm1.begin(1000, 500);
    pwm1.set_oc_polarity(1);
    pwm2.begin(1000, 800);
    pwm2.set_oc_polarity(1);
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
	PA15.mode(AF_PP);
	PB3.mode(AF_PP);  
    
    uart1.printf("max frq = %dKhz\r\n",pwm1.get_max_frq()/1000);
    uart1.printf("max frq = %f\r\n",pwm1.get_accuracy());
    PB9.mode(OUTPUT_PP);
}

float x;
uint16_t y;

int main(void)
{
    setup();

    while(1)
    {
        PB9.toggle();
        x = x + PI * 0.01;
        if(x >= PI)x = 0;
        y = 2000 - (sin(x) + 1) * 1000;

        pwm1.set_duty(y);
        delay_ms(10);
    }

}




