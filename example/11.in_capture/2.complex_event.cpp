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
	*	1	此例程演示了输入捕获高级模式的使用方法。支持频率周期，支持了占空比的采样
    *        输入捕获实验-测量周期，频率，占空比
    *   2   本例程为使用输入捕获模式测量一个PWM信号的周期，频率和占空比
    *   3   请将PA0和PB6使用跳线链接起来
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"InCapture complex example"
#define EXAMPLE_DATE	"2018-08-01"


InCapture ic0(&PA0);//创建一个输入捕获的对象
Pwm pwm1(&PB6);//创建一个PWM输出对象

//InCapture ic0(TIM1CH2);
//Pwm pwm1(TIM3CH1);

uint32_t frq = 0;

uint16_t p;
void cmp()
{
    ic0.complex_event();
}
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    ic0.begin(1); //初始化输入捕获参数，p分频
    ic0.attach(cmp);
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
            ic0.update_resault();
            UART.printf("peroid    = %0.2fus\r\n", ic0.res.peroid);
            UART.printf("frq(%d)= %0.2fhz\r\n", frq, ic0.res.frq);
            UART.printf("high_duty = %0.2f%%\r\n", ic0.res.high_duty);
            UART.printf("low duty  = %0.2f%%\r\n\r\n", ic0.res.low_duty);
        }
        pwm1.set_frq(frq += 100);
        delay_ms(3000);
    }
}








