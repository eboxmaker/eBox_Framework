/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

/*
������Ϊʹ�����벶��ģʽ����һ��PWM�źŵ����ں�Ƶ��
*/
#include "ebox.h"

IN_CAPTURE ic(&PA0);//����һ�����벶��Ķ���
PWM pwm1(&PB8);//����һ��PWM�������

uint32_t value1;
uint32_t value2;

void mesure_frq()//���벶���ж��¼�
{
    ic.set_count(0);
    value1 = ic.get_capture() + 170;
}
void update_event()
{
    ic.overflow_event_process();
}
uint16_t p;
void setup()
{
	ebox_init();
	uart1.begin(9600);
	PB9.mode(OUTPUT_PP);
    
    p = 1;
    ic.begin(p);//��ʼ�����벶�������p��Ƶ
    ic.attch_ic_interrupt(mesure_frq);//�󶨲����ж��¼�����
    ic.attch_update_interrupt(update_event);
    pwm1.begin(1000,900);
    pwm1.set_oc_polarity(1);
   
}

int main(void)
{
	setup();
	while(1)
    {
        PB9 = !PB9;

        if(value1)
        {
            uart1.printf("overtimes = %d\r\n",ic.get_overflow_state());
            uart1.printf("value1 = %d\r\n",value1);//���PWM����
            uart1.printf("frq = %0.0f\r\n",(72000000.0/p)/(value1));//���PWMƵ��
            value1 = 0;
        }
	}


}








