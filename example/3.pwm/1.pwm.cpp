/**
  ******************************************************************************
  * @file    pwm.cpp
  * @author  shentq
  * @version V2.0
  * @date    2016/08/14
  * @brief   ebox application example .
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/


#include "ebox.h"
#include "bsp_ebox.h"
/**
	*	1	��������ʾ��PWM����
	*	2	PWM���Ƶ��1Khz,ռ�ձȿɵ��ľ��β�
	*	3	ʵ������ʱLED����
	*   4	������Դ��ο�ebox_pwm.h˵��
	*/
	

/* ���������������̷������� */
#define EXAMPLE_NAME	"PWM example"
#define EXAMPLE_DATE	"2018-08-08"


float x;
uint16_t y;
Pwm pwm1(&PWMLED);
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

    pwm1.begin(1000, 500);
    pwm1.set_oc_polarity(1);//set output polarity after compare
    UART.printf("max frq = %dKhz\r\n",pwm1.get_max_frq()/1000);
    UART.printf("max frq = %f\r\n",pwm1.get_accuracy());
}
int main(void)
{
    setup();
    while(1)
    {
        x = x + PI * 0.01;
        if(x >= PI)x = 0;
        y = 2000 - (sin(x) + 1) * 1000;
        pwm1.set_duty(y);
        delay_ms(10);
    }
}




