/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

/*
输入捕获实验-测量周期和频率
本例程为使用输入捕获模式测量一个PWM信号的周期和频率
请将PA0和PB6使用跳线链接起来
*/
#include "ebox.h"

IN_CAPTURE ic0(&PA0);//创建一个输入捕获的对象
PWM pwm1(&PB6);//创建一个PWM输出对象


uint32_t us;


void m_frq_0()//输入捕获中断事件
{
    us = ic0.get_zone_time_us();//获取两个边沿之间的时间。
}

uint16_t p;
void setup()
{
    ebox_init();
    uart1.begin(115200);

    ic0.begin(1);//初始化输入捕获参数，p分频
    ic0.attch_ic_interrupt(m_frq_0);//绑定捕获中断事件函数

    pwm1.begin(1000, 900);
    pwm1.set_oc_polarity(1);

}

int main(void)
{
    setup();
    while(1)
    {
        while(us == 0);//等待产生数据
        uart1.printf("PWM周期 = %dus\r\n", us); //输出PWM周期
        uart1.printf("PWM频率 = %0.1fhz\r\n", 1000000.0/ (us)); //输出PWM频率
        us = 0;
        delay_ms(1000);
    }
}








