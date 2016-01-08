/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox

#include "ebox.h"

IN_CAPTURE ic(&PA0);
PWM pwm1(&PB8);

uint16_t value1;
uint16_t value2;
void test()
{
    ic.set_count(0);
    if(ic.polarity == TIM_ICPOLARITY_FALLING)
    {
        value1 = ic.get_capture();
        ic.set_polarity_rising();
    }
    else
    {
         value2 = ic.get_capture();
        ic.set_polarity_falling();
   }
}
uint32_t frq;
void setup()
{
	ebox_init();
	uart1.begin(9600);
	PB8.mode(OUTPUT_PP);
    frq = 1;
    ic.begin(frq);
    ic.attch_interrupt(test);
    pwm1.begin(10000,400);
    pwm1.set_oc_polarity(0);
   
}

int main(void)
{
	setup();
	while(1)

    {
//        if(value1 && value2)
//        {
//            uart1.printf("value1 = %d\r\n",value1);
//            uart1.printf("value2 = %d\r\n",value2);
//            uart1.printf("frq = %0.0f\r\n",frq*1.0/(value1+value2));
//            uart1.printf("pluse = %0.2f\r\n",value1*100.0/(value1+value2));
//            value1 = 0;
//            value2 = 0;
//        }
        if(value1)
        {
            uart1.printf("value1 = %d\r\n",value1);
            uart1.printf("frq = %0.0f\r\n",(72000000.0/frq)/(value1));
            value1 = 0;
        }
	}


}








