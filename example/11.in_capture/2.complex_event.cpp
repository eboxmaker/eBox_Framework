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
Pwm pwm1(&PB6);//����һ��PWM�������

uint32_t frq =0;
void measure_0()//���벶���ж��¼�
{
  ic0.complex_event();

}
uint16_t p;
void setup()
{
    ebox_init();
    uart1.begin(115200);

    ic0.begin(1);//��ʼ�����벶�������p��Ƶ
    ic0.attach(measure_0);//�󶨲����ж��¼�����
    uart1.printf("get_detect_min_pulse_us = %d\r\n",ic0.get_detect_min_pulse_us());
    frq = 1000;
    pwm1.begin(frq, 30);

}

int main(void)
{
    setup();
    while(1)
    {

        if(ic0.available())
        {              
            uart1.printf("peroid    = %0.2fus\r\n",ic0.get_wave_peroid());
            uart1.printf("frq(%d)= %0.2fhz\r\n",frq,ic0.get_wave_frq());
            uart1.printf("high_duty = %0.2f%%\r\n", ic0.get_wave_high_duty());
            uart1.printf("low duty  = %0.2f%%\r\n\r\n", ic0.get_wave_low_duty());
        }
        delay_ms(1000);

    }
}








