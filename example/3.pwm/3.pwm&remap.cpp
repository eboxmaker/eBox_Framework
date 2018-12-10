/**
  ******************************************************************************
  * @file    pwm.cpp
  * @author  shentq
  * @version V2.2
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
#include "bsp_ebox.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"PWM example"
#define EXAMPLE_DATE	"2018-08-08"

Pwm pwm1(&PA0);
Pwm pwm2(&PA1);

void setup()
{
    ebox_init();
    uart1.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);


    pwm1.begin(1000, 500);
    pwm1.set_oc_polarity(1);
    pwm2.begin(1000, 800);
    pwm2.set_oc_polarity(1);
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
    PA15.mode(AF_PP);
    PB3.mode(AF_PP);

    uart1.printf("max frq = %dKhz\r\n", pwm1.get_max_frq() / 1000);
    uart1.printf("max frq = %f\r\n", pwm1.get_accuracy());
    LED2.mode(OUTPUT_PP);
}

float x;
uint16_t y;

int main(void)
{
    setup();

    while(1)
    {
        LED2.toggle();
        x = x + PI * 0.01;
        if(x >= PI)x = 0;
        y = 2000 - (sin(x) + 1) * 1000;

        pwm1.set_duty(y);
        delay_ms(10);
    }

}




