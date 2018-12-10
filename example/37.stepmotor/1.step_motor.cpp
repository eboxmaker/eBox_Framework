/**
  ******************************************************************************
  * @file    pwm.cpp
  * @author  shentq
  * @version V2.0
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


StepMotor motor(TIM4, &PB8);
StepMotor motor1(TIM4, &PB9);
float x;
uint16_t y;
//Pwm pwm1(&PB8);

void setup()
{
    ebox_init();
    uart1.begin(115200);
    PB9.mode(OUTPUT_PP);
    PB9.set();
    motor.begin();
    motor1.begin();
    //    motor.move(50,100,100,1500);
    motor1.move(500, 100, 100, 1500);
    //    pwm1.begin(1000, 500);
    //    pwm1.set_oc_polarity(1);//set output polarity after compare
    //    uart1.printf("max frq = %dKhz\r\n",pwm1.get_max_frq()/1000);
    //    uart1.printf("max frq = %f\r\n",pwm1.get_accuracy());
    //    motor.move(-2000,100,100,2000);
}
int main(void)
{
    setup();
    while(1)
    {
        //    motor1.move(200,200,200,200);
        //        delay_ms(2500);
        //    motor1.move(200,200,200,2000);
        ////        delay_ms(2500);
        //    motor.move(100,100,100,1000);
        //        delay_ms(8*1000);
        //        sqrt(1.0);
        //        x = x + PI * 0.01;
        //        if(x >= PI)x = 0;
        //        y = 2000 - (sin(x) + 1) * 1000;
        //        motor.set_duty(y);
        //        delay_ms(10);
    }
}




