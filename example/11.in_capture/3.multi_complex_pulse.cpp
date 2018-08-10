/**
  ******************************************************************************
  * @file    *.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * Copyright 2015 shentq. All Rights Reserved.
  *
  * Copyright Notice
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq.
  *
  * @Disclaimer
  * This specification is preliminary and is subject to change at any time without
  * notice. shentq assumes no responsibility for any errors contained herein.
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
/*
���벶��ʵ��-�������ں�Ƶ��
������Ϊʹ�����벶��ģʽ����һ��PWM�źŵ����ں�Ƶ��
�뽫PA0��PB6ʹ��������������
*/
#include "ebox.h"

InCapture ic0(&PA0);//����һ�����벶��Ķ���
InCapture ic1(&PA1);//����һ�����벶��Ķ���
InCapture ic2(&PA2);//����һ�����벶��Ķ���
InCapture ic3(&PA3);//����һ�����벶��Ķ���

Pwm pwm1(&PB6);//����һ��PWM�������
Pwm pwm2(&PB7);//����һ��PWM�������
Pwm pwm3(&PB8);//����һ��PWM�������
Pwm pwm4(&PB9);//����һ��PWM�������

void measure_0()//���벶���ж��¼�
{
  ic0.complex_event();

}
void measure_1()//���벶���ж��¼�
{
  ic1.complex_event();

}
void measure_2()//���벶���ж��¼�
{
  ic2.complex_event();

}
void measure_3()//���벶���ж��¼�
{
  ic3.complex_event();

}
uint16_t p;
void setup()
{
    ebox_init();
    uart1.begin(115200);

    p = 1;
    ic0.begin(p);//��ʼ�����벶�������p��Ƶ
    ic0.attach(measure_0);//�󶨲����ж��¼�����
    ic1.begin(p);//��ʼ�����벶�������p��Ƶ
    ic1.attach(measure_1);//�󶨲����ж��¼�����
    ic2.begin(p);//��ʼ�����벶�������p��Ƶ
    ic2.attach(measure_2);//�󶨲����ж��¼�����
    ic3.begin(p);//��ʼ�����벶�������p��Ƶ
    ic3.attach(measure_3);//�󶨲����ж��¼�����

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

/**
  ******************************************************************************
  * @file    *.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * Copyright 2015 shentq. All Rights Reserved.
  *
  * Copyright Notice
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq.
  *
  * @Disclaimer
  * This specification is preliminary and is subject to change at any time without
  * notice. shentq assumes no responsibility for any errors contained herein.
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/

#include "ebox.h"
#include "bsp_ebox.h"

/**
	*	1	��������ʾ�������·����߼�,��ģʽ���ʹ�÷�����֧��Ƶ�����ڣ�֧����ռ�ձȵĲ���
    *        ���벶��ʵ��-�������ڣ�Ƶ�ʣ�ռ�ձ�
    *   2   ������Ϊʹ�����벶��ģʽ����һ��PWM�źŵ����ڣ�Ƶ�ʺ�ռ�ձ�
    *   3   �뽫PA0��PB6ʹ��������������
	*/


/* ���������������̷������� */
#define EXAMPLE_NAME	"InCapture multi channel complex and simple mode  example"
#define EXAMPLE_DATE	"2018-08-01"


InCapture ic0(&PA0);//����һ�����벶��Ķ���
InCapture ic1(&PA1);//����һ�����벶��Ķ���
InCapture ic2(&PA2);//����һ�����벶��Ķ���
InCapture ic3(&PA3);//����һ�����벶��Ķ���

Pwm pwm1(&PB6);//����һ��PWM�������
Pwm pwm2(&PB7);//����һ��PWM�������
Pwm pwm3(&PB8);//����һ��PWM�������
Pwm pwm4(&PB9);//����һ��PWM�������

uint32_t frq =0;

uint16_t p;
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

    ic0.begin(1,SIMPLE);//��ʼ�����벶�������p��Ƶ
    ic1.begin(1,COMPLEX);//��ʼ�����벶�������p��Ƶ
    ic2.begin(1,SIMPLE);//��ʼ�����벶�������p��Ƶ
    ic3.begin(1,COMPLEX);//��ʼ�����벶�������p��Ƶ
    
    UART.printf("get_detect_min_pulse_us = %d\r\n",ic0.get_detect_min_pulse_us());
    frq = 1000;
    
    
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

        if(ic0.available())
        {              
            uart1.printf("peroid0    = %0.2fus\r\n",ic0.get_wave_peroid());
            uart1.printf("frq0       = %0.2fhz\r\n",ic0.get_wave_frq());
            uart1.printf("high_duty0 = %0.2f%%\r\n", ic0.get_wave_high_duty());
            uart1.printf("low duty0  = %0.2f%%\r\n\r\n", ic0.get_wave_low_duty());
        }
        if(ic1.available())
        {              
            uart1.printf("peroid1    = %0.2fus\r\n",ic1.get_wave_peroid());
            uart1.printf("frq1       = %0.2fhz\r\n",ic1.get_wave_frq());
            uart1.printf("high_duty1 = %0.2f%%\r\n", ic1.get_wave_high_duty());
            uart1.printf("low duty1  = %0.2f%%\r\n\r\n", ic1.get_wave_low_duty());
        }
        if(ic2.available())
        {              
            uart1.printf("peroid2    = %0.2fus\r\n",ic2.get_wave_peroid());
            uart1.printf("frq2       = %0.2fhz\r\n",ic2.get_wave_frq());
            uart1.printf("high_duty2 = %0.2f%%\r\n", ic2.get_wave_high_duty());
            uart1.printf("low duty2  = %0.2f%%\r\n\r\n", ic2.get_wave_low_duty());
        }
        if(ic3.available())
        {              
            uart1.printf("peroid3    = %0.2fus\r\n",ic3.get_wave_peroid());
            uart1.printf("frq3       = %0.2fhz\r\n",ic3.get_wave_frq());
            uart1.printf("high_duty3 = %0.2f%%\r\n", ic3.get_wave_high_duty());
            uart1.printf("low duty3  = %0.2f%%\r\n\r\n", ic3.get_wave_low_duty());
        }
        delay_ms(1000);
    }
}









    }
}








