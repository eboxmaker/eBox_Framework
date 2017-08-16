/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "../common/apps/pid/pid_v1.h"

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp=0.01, Ki= 1000, Kd=0.01;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd,P_ON_E, DIRECT);


Pwm pwm(&PA2);


void setup()
{
    double Answer[5], SquarePoor[4];
    ebox_init();
    uart1.begin(115200);
    uart1.printf("\r\nuart1 115200 ok!\r\n");
    PB1.mode(AIN);
    pwm.begin(1000,500);
    pwm.set_oc_polarity(0);//set output polarity after compare
  //initialize the variables we're linked to
  Input = analog_read(&PB1);
  Setpoint = 350;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}


int main(void)
{
    static uint64_t last_time = millis();
    static uint64_t last_time1 = millis();
    setup();

    while(1)
    {
        if(millis() - last_time > 1)
        {
            last_time = millis();
            Input =(analog_read(&PB1)>>3);
            myPID.Compute();
            pwm.set_duty(Output);
        }
        if(millis() - last_time1 > 20)
        {
            last_time1 = millis();
            uart1.printf("in = %0.2f,\t out = %0.2f\r\n",Input,Output);
        }                    

        //delay_ms(100);
    }
}




