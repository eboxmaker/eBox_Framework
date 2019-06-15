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


Pwm pwm1(&PB6);//创建一个PWM输出对象


void measure_0()//输入捕获中断事件
{
    ic0.complex_event();
    
    if(ic0.polarity == TIM_ICPolarity_Falling)
        uart1.printf("falling\r\n");
    else
        uart1.printf("rising\r\n");
    
    ic0.update_resault();
    uart1.printf("peroid0    = %0.2fus\r\n", ic0.res.peroid);
    uart1.printf("frq0       = %0.2fhz\r\n", ic0.res.frq);
    uart1.printf("high_duty0 = %0.2f%%\r\n", ic0.res.high_duty);
    uart1.printf("low duty0  = %0.2f%%\r\n\r\n", ic0.res.low_duty);
}
uint16_t p;
void setup()
{
    ebox_init();
    uart1.begin(115200);
    pwm1.begin(1, 900);
    pwm1.set_oc_polarity(1);
    
    ic0.begin(72);//初始化输入捕获参数，p分频
    ic0.attach(measure_0);//绑定捕获中断事件函数



}

int main(void)
{
    setup();
    while(1)
    {


    }
}








