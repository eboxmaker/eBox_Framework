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

IN_CAPTURE ic0(&PA0);//����һ�����벶��Ķ���
PWM pwm1(&PB6);//����һ��PWM�������


uint32_t value0;


void m_frq_0()//���벶���ж��¼�
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
    ic0.begin(p);//��ʼ�����벶�������p��Ƶ
    ic0.attch_ic_interrupt(m_frq_0);//�󶨲����ж��¼�����

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
            uart1.printf("value0 = %d\r\n", value0); //���PWM����
            uart1.printf("frq = %0.0f\r\n", (72000000.0 / p) / (value0)); //���PWMƵ��
            value0 = 0;
        }
    }
}








