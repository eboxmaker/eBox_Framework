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

IN_CAPTURE ic(&PA0);//创建一个输入捕获的对象
PWM pwm1(&PB8);//创建一个PWM输出对象

uint16_t value1;
uint16_t value2;

void mesure_frq()//输入捕获中断事件
{
    ic.set_count(0);
    value1 = ic.get_capture()+5;

}
uint16_t p;
void setup()
{
	ebox_init();
	uart1.begin(9600);
	PB8.mode(OUTPUT_PP);
    
    p = 36;
    ic.begin(p);//初始化输入捕获参数，p分频
    ic.attch_interrupt(mesure_frq);//绑定捕获中断事件函数
    
    pwm1.begin(1000,500);
    pwm1.set_oc_polarity(1);
   
}

int main(void)
{
	setup();
	while(1)
    {

        if(value1)
        {
            uart1.printf("value1 = %d\r\n",value1);//输出PWM周期
            uart1.printf("frq = %0.0f\r\n",(72000000.0/p)/(value1));//输出PWM频率
            value1 = 0;
        }
	}


}








