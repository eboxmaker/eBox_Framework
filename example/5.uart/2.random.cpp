 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   ebox application example .
	*		 2018-8-5	ͨ������bsp������Ӳ���˿ڣ��������̿����ڲ�ͬƽ̨������
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
	*	1	��������ʾ��β��������
	*/
/* ���������������̷������� */
#define EXAMPLE_NAME	"random out example"
#define EXAMPLE_DATE	"2018-08-06"

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

}
int main(void)
{
    setup();
    while(1)
    {
        UART.printf("random=%d\r\n", random());							// 0-RAND_MAX(0x7fffffff)�������,������stdlib.h��
        UART.printf("random=%d\r\n", random(100));					// 100���ڵ������
        UART.printf("random=%d\r\n", random(100, 65535));		// 100-65535֮��������
        delay_ms(1000);
    }
}




