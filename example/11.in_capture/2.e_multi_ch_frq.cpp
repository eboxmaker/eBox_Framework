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
IN_CAPTURE ic1(&PA1);//����һ�����벶��Ķ���
IN_CAPTURE ic2(&PA2);//����һ�����벶��Ķ���
IN_CAPTURE ic3(&PA3);//����һ�����벶��Ķ���
PWM pwm1(&PB6);//����һ��PWM�������
PWM pwm2(&PB7);//����һ��PWM�������

uint32_t value0;
uint32_t value1;
uint32_t value2;
uint32_t value3;

void m_frq_0()//���벶���ж��¼�
{
    value0 = ic0.get_capture();
}
void m_frq_1()//���벶���ж��¼�
{
    value1 = ic1.get_capture();
}
void m_frq_2()//���벶���ж��¼�
{
    value2 = ic2.get_capture();
}
void m_frq_3()//���벶���ж��¼�
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
    ic0.begin(p);//��ʼ�����벶�������p��Ƶ
    ic0.attch_ic_interrupt(m_frq_0);//�󶨲����ж��¼�����
    ic1.begin(p);//��ʼ�����벶�������p��Ƶ
    ic1.attch_ic_interrupt(m_frq_1);//�󶨲����ж��¼�����
    ic2.begin(p);//��ʼ�����벶�������p��Ƶ
    ic2.attch_ic_interrupt(m_frq_2);//�󶨲����ж��¼�����
    ic3.begin(p);//��ʼ�����벶�������p��Ƶ
    ic3.attch_ic_interrupt(m_frq_3);//�󶨲����ж��¼�����




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
            uart1.printf("value0 = %d\r\n", value0); //���PWM����
            uart1.printf("frq = %0.0f\r\n", (72000000.0 / p) / (value0)); //���PWMƵ��
            value0 = 0;
        }
        if(value1)
        {
            uart1.printf("value1 = %d\r\n", value1); //���PWM����
            uart1.printf("frq = %0.0f\r\n", (72000000.0 / p) / (value1)); //���PWMƵ��
            value1 = 0;
        }
        if(value2)
        {
            uart1.printf("value1 = %d\r\n", value2); //���PWM����
            uart1.printf("frq = %0.0f\r\n", (72000000.0 / p) / (value2)); //���PWMƵ��
            value2 = 0;
        }
        if(value3)
        {
            uart1.printf("value1 = %d\r\n", value3); //���PWM����
            uart1.printf("frq = %0.0f\r\n", (72000000.0 / p) / (value3)); //���PWMƵ��
            value3 = 0;
        }
    }
}








