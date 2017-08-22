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
#include "math.h"
#include "ebox_encoder.h"
#include "pid_v1.h"

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp=20, Ki= 550, Kd=0.1;
PID myPID(&Input, &Output, &Setpoint);


Encoder encoder(TIM4,&PB6,&PB7);
float x;
uint16_t y;
Pwm pwm1(&PA0);
void setup()
{
    ebox_init();
    uart1.begin(115200);
    encoder.begin(40000,3);
    pwm1.begin(2000, 1000);
    pwm1.set_oc_polarity(1);//set output polarity after compare
    
    //确定采样控制周期
//    while(1)
//    {         
//        Input  = encoder.read_speed();
//        uart1.printf("%0.2f\t%d\r\n",Input,millis());
//        delay_ms(5);
//    }
    uart1.printf("max frq = %dKhz\r\n",pwm1.get_max_frq()/1000);
    uart1.printf("max frq = %f\r\n",pwm1.get_accuracy());
    
    //initialize the variables we're linked to
    Input = 0;
    Setpoint = 27.5;

    //turn the PID on
    myPID.begin(Kp, Ki, Kd, DIRECT,AUTOMATIC,5,0,1000);
}
int main(void)
{
    static uint64_t last_time = millis();
    static uint64_t last_time1 = millis();
    setup();

    while(1)
    {

        //设置采样和控制周期为5ms
        if(millis() - last_time > 5)
        {
            last_time = millis();
            Input  = encoder.read_speed();
            pwm1.set_duty(Output);
            uart1.printf("%0.2f\t%0.2f\r\n",Input,Output);
        }
        myPID.Compute();      
    }
}




