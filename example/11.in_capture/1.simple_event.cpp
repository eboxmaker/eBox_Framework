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
	*	1	��������ʾ�����벶���ģʽ��ʹ�÷�����֧��Ƶ�����ڣ���֧����ռ�ձȵĲ���
    *        ���벶��ʵ��-�������ں�Ƶ��
    *   2   ������Ϊʹ�����벶��ģʽ����һ��PWM�źŵ����ں�Ƶ��
    *   3   �뽫PA0��PB6ʹ��������������
	*/


/* ���������������̷������� */
#define EXAMPLE_NAME	"InCapture simple example"
#define EXAMPLE_DATE	"2018-08-01"


InCapture ic0(&PA0);//����һ�����벶��Ķ���
Pwm pwm1(&PB6);//����һ��PWM�������


uint32_t frq = 0;

uint16_t p;
void simple()
{
    ic0.simple_event();
}
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    ic0.begin(1); //��ʼ�����벶�������p��Ƶ
    ic0.attach(simple);
    UART.flush();
    frq = 100*1000;
    pwm1.begin(frq, 500);
}

int main(void)
{
    setup();
    while(1)
    {
        if(ic0.available())
        {
            ic0.update_resault();
            UART.printf("peroid    = %0.2fus\r\n", ic0.res.peroid);
            UART.printf("frq(%d)= %0.2fhz\r\n", frq, ic0.res.frq);
            UART.printf("high_duty = %0.2f%%\r\n", ic0.res.high_duty);
            UART.printf("low duty  = %0.2f%%\r\n\r\n", ic0.res.low_duty);
        }
        frq+=100;
        pwm1.set_frq(frq);
        delay_ms(1000);
    }
}








