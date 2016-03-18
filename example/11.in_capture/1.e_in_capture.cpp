/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

/*
本例程为使用输入捕获模式测量一个PWM信号的周期和频率
*/
#include "ebox.h"

IN_CAPTURE ic0(&PA0);//创建一个输入捕获的对象
PWM pwm1(&PB6);//创建一个PWM输出对象


uint32_t value0;


void m_frq_0()//输入捕获中断事件
{
    value0 = ic0.get_capture();
}

uint16_t p;
void setup()
{
    ebox_init();
    uart1.begin(9600);
    PB9.mode(OUTPUT_PP);

    p = 1;
    ic0.begin(p);//初始化输入捕获参数，p分频
    ic0.attch_ic_interrupt(m_frq_0);//绑定捕获中断事件函数

    pwm1.begin(1200, 900);
    pwm1.set_oc_polarity(1);

}

int main(void)
{
    setup();
    while(1)
    {
        PB9 = !PB9;

        if(value0)
        {
            uart1.printf("value0 = %d\r\n", value0); //输出PWM周期
            uart1.printf("frq = %0.0f\r\n", (72000000.0 / p) / (value0)); //输出PWM频率
            value0 = 0;
        }
    }
}








