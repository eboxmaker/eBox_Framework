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
PWM pwm3(&PB8);//创建一个PWM输出对象
PWM pwm4(&PB9);//创建一个PWM输出对象


uint32_t value00;
uint32_t value01;
uint32_t value10;
uint32_t value11;
uint32_t value20;
uint32_t value21;
uint32_t value30;
uint32_t value31;


void measure_0()//输入捕获中断事件
{
    if(ic0.polarity == TIM_ICPOLARITY_FALLING)//检测到下降沿，测量高电平时间完成
    {
        value00 = ic0.get_capture();
        ic0.set_polarity_rising();//切换检测上升沿
    }
    else//检测到上升沿，测量低电平时间完成
    {
         value01 = ic0.get_capture();
        ic0.set_polarity_falling();//切换检测下降沿
   }
}
void measure_1()//输入捕获中断事件
{
    if(ic1.polarity == TIM_ICPOLARITY_FALLING)//检测到下降沿，测量高电平时间完成
    {
        value10 = ic1.get_capture();
        ic1.set_polarity_rising();//切换检测上升沿
    }
    else//检测到上升沿，测量低电平时间完成
    {
         value11 = ic1.get_capture();
        ic1.set_polarity_falling();//切换检测下降沿
   }
}
void measure_2()//输入捕获中断事件
{
    if(ic2.polarity == TIM_ICPOLARITY_FALLING)//检测到下降沿，测量高电平时间完成
    {
        value20 = ic2.get_capture();
        ic2.set_polarity_rising();//切换检测上升沿
    }
    else//检测到上升沿，测量低电平时间完成
    {
         value21 = ic2.get_capture();
        ic2.set_polarity_falling();//切换检测下降沿
   }
}
void measure_3()//输入捕获中断事件
{
    if(ic3.polarity == TIM_ICPOLARITY_FALLING)//检测到下降沿，测量高电平时间完成
    {
        value30 = ic3.get_capture();
        ic3.set_polarity_rising();//切换检测上升沿
    }
    else//检测到上升沿，测量低电平时间完成
    {
         value31 = ic3.get_capture();
        ic3.set_polarity_falling();//切换检测下降沿
   }
}

uint16_t p;
void setup()
{
	ebox_init();
	uart1.begin(9600);
    
    p = 1;
    ic0.begin(p);//初始化输入捕获参数，p分频
    ic0.attch_ic_interrupt(measure_0);//绑定捕获中断事件函数
    ic1.begin(p);//初始化输入捕获参数，p分频
    ic1.attch_ic_interrupt(measure_1);//绑定捕获中断事件函数
    ic2.begin(p);//初始化输入捕获参数，p分频
    ic2.attch_ic_interrupt(measure_2);//绑定捕获中断事件函数
    ic3.begin(p);//初始化输入捕获参数，p分频
    ic3.attch_ic_interrupt(measure_3);//绑定捕获中断事件函数

    pwm1.begin(50,900);
    pwm1.set_oc_polarity(1);
    pwm2.begin(50,850);
    pwm2.set_oc_polarity(1);
    pwm3.begin(50,603);
    pwm3.set_oc_polarity(1);
    pwm4.begin(50,201);
    pwm4.set_oc_polarity(1);
   
}

int main(void)
{
	setup();
	while(1)
    {

        if(value00 && value01)
        {
//            uart1.printf("value1 = %d\r\n",value00);
//            uart1.printf("value2 = %d\r\n",value01);
//            uart1.printf("frq_0 = %0.0f\r\n",(72000000.0/p)/(value00+value01));
            uart1.printf("pluse_0 = %0.2f\r\n",value00*100.0/(value00+value01));
            value00 = 0;
            value01 = 0;
        }       

        if(value10 && value11)
        {
//            uart1.printf("value1 = %d\r\n",value10);
//            uart1.printf("value2 = %d\r\n",value11);
//            uart1.printf("frq_1 = %0.0f\r\n",(72000000.0/p)/(value10+value11));
            uart1.printf("pluse_1 = %0.2f\r\n",value10*100.0/(value10+value11));
            value10 = 0;
            value11 = 0;
        }   
        
        if(value20 && value21)
        {
//            uart1.printf("value1 = %d\r\n",value20);
//            uart1.printf("value2 = %d\r\n",value21);
//            uart1.printf("frq_2 = %0.0f\r\n",(72000000.0/p)/(value20+value21));
            uart1.printf("pluse_2 = %0.2f\r\n",value20*100.0/(value20+value21));
            value20 = 0;
            value21 = 0;
        }       

        if(value30 && value31)
        {
//            uart1.printf("value1 = %d\r\n",value30);
//            uart1.printf("value2 = %d\r\n",value31);
            uart1.printf("pluse_3 = %0.2f\r\n",value30*100.0/(value30+value31));
            uart1.printf("frq_3 = %0.0f\r\n",(72000000.0/p)/(value30+value31));
            value30 = 0;
            value31 = 0;
        }       
	}
}








