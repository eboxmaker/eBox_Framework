/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *					 2018-8-5	ͨ������bsp������Ӳ���˿ڣ��������̿����ڲ�ͬƽ̨������ 
	* Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */



#include "ebox.h"
#include "math.h"
#include "bsp_ebox.h"

/**
	*	1	��������ʾ��ģ��DAC����
	*	2	ʵ��������PWM���Ƶ��1Khz��ռ�ձȿɵ��ķ�����0-1000��
	*	3	ʵ������ʱLED����
	*   4	������Դ��ο�ebox_pwm.h˵��
	*/
	

/* ���������������̷������� */
#define EXAMPLE_NAME	"Analog_Write example"
#define EXAMPLE_DATE	"2018-08-05"



void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
}

float x;
uint16_t y;
int main(void)
{

    setup();

    while(1)
    {
        x = x + PI * 0.01;
        if(x >= PI)x = 0;
        y = 2000 - (sin(x) + 1) * 1000;

        analog_write(&DAC_0, y);
        delay_ms(20);
    }
}
