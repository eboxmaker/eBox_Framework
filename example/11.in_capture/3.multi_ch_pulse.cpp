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
PWM pwm3(&PB8);//����һ��PWM�������
PWM pwm4(&PB9);//����һ��PWM�������


uint32_t value00;
uint32_t value01;
uint32_t value10;
uint32_t value11;
uint32_t value20;
uint32_t value21;
uint32_t value30;
uint32_t value31;


void measure_0()//���벶���ж��¼�
{
    if(ic0.polarity == TIM_ICPOLARITY_FALLING)//��⵽�½��أ������ߵ�ƽʱ�����
    {
        value00 = ic0.get_capture();
        ic0.set_polarity_rising();//�л����������
    }
    else//��⵽�����أ������͵�ƽʱ�����
    {
        value01 = ic0.get_capture();
        ic0.set_polarity_falling();//�л�����½���
    }
}
void measure_1()//���벶���ж��¼�
{
    if(ic1.polarity == TIM_ICPOLARITY_FALLING)//��⵽�½��أ������ߵ�ƽʱ�����
    {
        value10 = ic1.get_capture();
        ic1.set_polarity_rising();//�л����������
    }
    else//��⵽�����أ������͵�ƽʱ�����
    {
        value11 = ic1.get_capture();
        ic1.set_polarity_falling();//�л�����½���
    }
}
void measure_2()//���벶���ж��¼�
{
    if(ic2.polarity == TIM_ICPOLARITY_FALLING)//��⵽�½��أ������ߵ�ƽʱ�����
    {
        value20 = ic2.get_capture();
        ic2.set_polarity_rising();//�л����������
    }
    else//��⵽�����أ������͵�ƽʱ�����
    {
        value21 = ic2.get_capture();
        ic2.set_polarity_falling();//�л�����½���
    }
}
void measure_3()//���벶���ж��¼�
{
    if(ic3.polarity == TIM_ICPOLARITY_FALLING)//��⵽�½��أ������ߵ�ƽʱ�����
    {
        value30 = ic3.get_capture();
        ic3.set_polarity_rising();//�л����������
    }
    else//��⵽�����أ������͵�ƽʱ�����
    {
        value31 = ic3.get_capture();
        ic3.set_polarity_falling();//�л�����½���
    }
}

uint16_t p;
void setup()
{
    ebox_init();
    uart1.begin(9600);

    p = 1;
    ic0.begin(p);//��ʼ�����벶�������p��Ƶ
    ic0.attch_ic_interrupt(measure_0);//�󶨲����ж��¼�����
    ic1.begin(p);//��ʼ�����벶�������p��Ƶ
    ic1.attch_ic_interrupt(measure_1);//�󶨲����ж��¼�����
    ic2.begin(p);//��ʼ�����벶�������p��Ƶ
    ic2.attch_ic_interrupt(measure_2);//�󶨲����ж��¼�����
    ic3.begin(p);//��ʼ�����벶�������p��Ƶ
    ic3.attch_ic_interrupt(measure_3);//�󶨲����ж��¼�����

    pwm1.begin(50, 900);
    pwm1.set_oc_polarity(1);
    pwm2.begin(50, 850);
    pwm2.set_oc_polarity(1);
    pwm3.begin(50, 603);
    pwm3.set_oc_polarity(1);
    pwm4.begin(50, 201);
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
            uart1.printf("pluse_0 = %0.2f\r\n", value00 * 100.0 / (value00 + value01));
            value00 = 0;
            value01 = 0;
        }

        if(value10 && value11)
        {
            //            uart1.printf("value1 = %d\r\n",value10);
            //            uart1.printf("value2 = %d\r\n",value11);
            //            uart1.printf("frq_1 = %0.0f\r\n",(72000000.0/p)/(value10+value11));
            uart1.printf("pluse_1 = %0.2f\r\n", value10 * 100.0 / (value10 + value11));
            value10 = 0;
            value11 = 0;
        }

        if(value20 && value21)
        {
            //            uart1.printf("value1 = %d\r\n",value20);
            //            uart1.printf("value2 = %d\r\n",value21);
            //            uart1.printf("frq_2 = %0.0f\r\n",(72000000.0/p)/(value20+value21));
            uart1.printf("pluse_2 = %0.2f\r\n", value20 * 100.0 / (value20 + value21));
            value20 = 0;
            value21 = 0;
        }

        if(value30 && value31)
        {
            //            uart1.printf("value1 = %d\r\n",value30);
            //            uart1.printf("value2 = %d\r\n",value31);
            uart1.printf("pluse_3 = %0.2f\r\n", value30 * 100.0 / (value30 + value31));
            uart1.printf("frq_3 = %0.0f\r\n", (72000000.0 / p) / (value30 + value31));
            value30 = 0;
            value31 = 0;
        }
    }
}








