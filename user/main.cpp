/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

/*
���벶��ʵ��-�������ں�Ƶ��
������Ϊʹ�����벶��ģʽ����һ��PWM�źŵ����ں�Ƶ��
�뽫PA0��PB6ʹ��������������
*/
#include "ebox.h"

IN_CAPTURE ic0(&PA0);//����һ�����벶��Ķ���
PWM pwm1(&PB6);//����һ��PWM�������

float us;
uint32_t count;

void m_frq_0()//���벶���ж��¼�
{
    //�����ڵ�Ƶ�ʲɼ����ŵ��������û�ȥ���㣬ȱ���Ƶ�ʵĵ��ø������㡣ռ��cpu����ʱ��
    us = ic0.get_zone_time_us();//��ȡ��������֮���ʱ�䡣
    //���������������ִ��Ч�ʸߣ�ȱ����Ҫ�û�����(us)=get_capture()/(72.0/prescaler);
    //count = ic0.get_capture();//��ȡ��������֮���ʱ�䡣
}

void setup()
{
    ebox_init();
    uart1.begin(115200);

    ic0.begin(1);//��ʼ�����벶�������p��Ƶ
    ic0.attch_ic_interrupt(m_frq_0);//�󶨲����ж��¼�����

    pwm1.begin(1000, 500);
    pwm1.set_oc_polarity(1);
}
int main(void)
{
    setup();
    while(1)
    {
        while(us == 0);//�ȴ���������
        uart1.printf("PWM���� = %fus\r\n", us); //���PWM����
        uart1.printf("PWMƵ�� = %0.1fhz\r\n", 1000000.0/ (us)); //���PWMƵ��
        us = 0;
        delay_ms(1000);
    }
}








