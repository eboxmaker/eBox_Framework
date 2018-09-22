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
	*	1	��������ʾ�����벶��߼�ģʽ��ʹ�÷�����֧��Ƶ�����ڣ�֧����ռ�ձȵĲ���
    *        ���벶��ʵ��-�������ڣ�Ƶ�ʣ�ռ�ձ�
    *   2   ������Ϊʹ�����벶��ģʽ����һ��PWM�źŵ����ڣ�Ƶ�ʺ�ռ�ձ�
    *   3   �뽫PA0��PB6ʹ��������������
	*/


/* ���������������̷������� */
#define EXAMPLE_NAME	"InCapture complex example"
#define EXAMPLE_DATE	"2018-08-01"


//InCapture ic0(&PA0);//����һ�����벶��Ķ���
//Pwm pwm1(&PB6);//����һ��PWM�������

InCapture ic0(TIM1CH2);
Pwm pwm1(TIM3CH1);

uint32_t frq =0;

uint16_t p;
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

    ic0.begin(1,COMPLEX);//��ʼ�����벶�������p��Ƶ
    UART.printf("get_detect_min_pulse_us = %d\r\n",ic0.get_detect_min_pulse_us());
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
            UART.printf("peroid    = %0.2fus\r\n",ic0.get_wave_peroid());
            UART.printf("frq(%d)= %0.2fhz\r\n",frq,ic0.get_wave_frq());
            UART.printf("high_duty = %0.2f%%\r\n", ic0.get_wave_high_duty());
            UART.printf("low duty  = %0.2f%%\r\n\r\n", ic0.get_wave_low_duty());
        }
        pwm1.set_frq(frq+=100);
        delay_ms(1000);        
    }
}








