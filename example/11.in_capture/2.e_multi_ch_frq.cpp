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
IN_CAPTURE ic1(&PA1);//创建一个输入捕获的对象
IN_CAPTURE ic2(&PA2);//创建一个输入捕获的对象
IN_CAPTURE ic3(&PA3);//创建一个输入捕获的对象
PWM pwm1(&PB6);//创建一个PWM输出对象
PWM pwm2(&PB7);//创建一个PWM输出对象

uint32_t value0;
uint32_t value1;
uint32_t value2;
uint32_t value3;

void m_frq_0()//输入捕获中断事件
{
    value0 = ic0.get_capture();
}
void m_frq_1()//输入捕获中断事件
{
    value1 = ic1.get_capture();
}
void m_frq_2()//输入捕获中断事件
{
    value2 = ic2.get_capture();
}
void m_frq_3()//输入捕获中断事件
{
    value3 = ic3.get_capture();
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
    ic1.begin(p);//初始化输入捕获参数，p分频
    ic1.attch_ic_interrupt(m_frq_1);//绑定捕获中断事件函数
    ic2.begin(p);//初始化输入捕获参数，p分频
    ic2.attch_ic_interrupt(m_frq_2);//绑定捕获中断事件函数
    ic3.begin(p);//初始化输入捕获参数，p分频
    ic3.attch_ic_interrupt(m_frq_3);//绑定捕获中断事件函数




    pwm1.begin(1200, 900);
    pwm1.set_oc_polarity(1);
    pwm2.begin(1200, 500);
    pwm2.set_oc_polarity(0);

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
        if(value1)
        {
            uart1.printf("value1 = %d\r\n", value1); //输出PWM周期
            uart1.printf("frq = %0.0f\r\n", (72000000.0 / p) / (value1)); //输出PWM频率
            value1 = 0;
        }
        if(value2)
        {
            uart1.printf("value1 = %d\r\n", value2); //输出PWM周期
            uart1.printf("frq = %0.0f\r\n", (72000000.0 / p) / (value2)); //输出PWM频率
            value2 = 0;
        }
        if(value3)
        {
            uart1.printf("value1 = %d\r\n", value3); //输出PWM周期
            uart1.printf("frq = %0.0f\r\n", (72000000.0 / p) / (value3)); //输出PWM频率
            value3 = 0;
        }
    }
}








