/**
 ******************************************************************************
 * @file    main.cpp
 * @author  shentq
 * @version V1.2
 * @date    2016/08/14
 * @brief   ebox application example .
*					 2018-8-5	ͨ������bsp������Ӳ���˿ڣ��������̿����ڲ�ͬƽ̨������
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

#include "ebox.h"
#include "bsp_ebox.h"

/**
	*	1	ͨ�����ڴ�ӡ��Ϣ
	*/
/* ���������������̷������� */
#define EXAMPLE_NAME	"hello world example"
#define EXAMPLE_DATE	"2018-08-06"

void setup()
{
    ebox_init();
    LED1.mode(OUTPUT_PP);
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
}
uint32_t mark1, mark2,mark3;
int main(void)
{
    
    setup();
    while(1)
    {
        mark1 = micros();
        delay_us(1000000);
        mark2 = micros();
        mark3 = mark2 -mark1;
        UART.printf("%u:%u\n",micros(),mark3);
    }
}




