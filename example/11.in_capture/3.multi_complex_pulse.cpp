/**
  ******************************************************************************
  * @file    *.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * Copyright 2015 shentq. All Rights Reserved.
  *
  * Copyright Notice
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq.
  *
  * @Disclaimer
  * This specification is preliminary and is subject to change at any time without
  * notice. shentq assumes no responsibility for any errors contained herein.
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
/*
输入捕获实验-测量周期和频率
本例程为使用输入捕获模式测量一个PWM信号的周期和频率
请将PA0和PB6使用跳线链接起来
*/
#include "ebox.h"

InCapture ic0(&PA0);//创建一个输入捕获的对象
InCapture ic1(&PA1);//创建一个输入捕获的对象
InCapture ic2(&PA2);//创建一个输入捕获的对象
InCapture ic3(&PA3);//创建一个输入捕获的对象

Pwm pwm1(&PB6);//创建一个PWM输出对象
Pwm pwm2(&PB7);//创建一个PWM输出对象
Pwm pwm3(&PB8);//创建一个PWM输出对象
Pwm pwm4(&PB9);//创建一个PWM输出对象

void measure_0()//输入捕获中断事件
{
  ic0.complex_event();

}
void measure_1()//输入捕获中断事件
{
  ic1.complex_event();

}
void measure_2()//输入捕获中断事件
{
  ic2.complex_event();

}
void measure_3()//输入捕获中断事件
{
  ic3.complex_event();

}
uint16_t p;
void setup()
{
    ebox_init();
    uart1.begin(115200);

    p = 1;
    ic0.begin(p);//初始化输入捕获参数，p分频
    ic0.attach(measure_0);//绑定捕获中断事件函数
    ic1.begin(p);//初始化输入捕获参数，p分频
    ic1.attach(measure_1);//绑定捕获中断事件函数
    ic2.begin(p);//初始化输入捕获参数，p分频
    ic2.attach(measure_2);//绑定捕获中断事件函数
    ic3.begin(p);//初始化输入捕获参数，p分频
    ic3.attach(measure_3);//绑定捕获中断事件函数

    pwm1.begin(50, 900);
    pwm1.set_oc_polarity(1);
    pwm2.begin(50, 850);
    pwm2.set_oc_polarity(1);
    pwm3.begin(50, 603);
    pwm3.set_oc_polarity(1);
    pwm4.begin(50, 201);
    pwm4.set_oc_polarity(1);

}

int main(void)
{
    setup();
    while(1)
    {

        if(ic0.available())
        {              
            uart1.printf("peroid0    = %0.2fus\r\n",ic0.get_wave_peroid());
            uart1.printf("frq0       = %0.2fhz\r\n",ic0.get_wave_frq());
            uart1.printf("high_duty0 = %0.2f%%\r\n", ic0.get_wave_high_duty());
            uart1.printf("low duty0  = %0.2f%%\r\n\r\n", ic0.get_wave_low_duty());
        }
        if(ic1.available())
        {              
            uart1.printf("peroid1    = %0.2fus\r\n",ic1.get_wave_peroid());
            uart1.printf("frq1       = %0.2fhz\r\n",ic1.get_wave_frq());
            uart1.printf("high_duty1 = %0.2f%%\r\n", ic1.get_wave_high_duty());
            uart1.printf("low duty1  = %0.2f%%\r\n\r\n", ic1.get_wave_low_duty());
        }
        if(ic2.available())
        {              
            uart1.printf("peroid2    = %0.2fus\r\n",ic2.get_wave_peroid());
            uart1.printf("frq2       = %0.2fhz\r\n",ic2.get_wave_frq());
            uart1.printf("high_duty2 = %0.2f%%\r\n", ic2.get_wave_high_duty());
            uart1.printf("low duty2  = %0.2f%%\r\n\r\n", ic2.get_wave_low_duty());
        }
        if(ic3.available())
        {              
            uart1.printf("peroid3    = %0.2fus\r\n",ic3.get_wave_peroid());
            uart1.printf("frq3       = %0.2fhz\r\n",ic3.get_wave_frq());
            uart1.printf("high_duty3 = %0.2f%%\r\n", ic3.get_wave_high_duty());
            uart1.printf("low duty3  = %0.2f%%\r\n\r\n", ic3.get_wave_low_duty());
        }
        delay_ms(1000);

    }
}








