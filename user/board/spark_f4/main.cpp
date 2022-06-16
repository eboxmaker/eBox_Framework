/**
 ******************************************************************************
 * @file    main.cpp
 * @author  cat_li
 * @version V1.0
 * @date    2017/07/13
 * @brief   ebox exti example, ����stm32nucleo(072)ƽ̨��֤
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
	*	1	��������ʾ��GPIO�ж�
    *   2   ����userbt1�����û����������º͵���󶨲�ͬ�Ļص�����
            ���𴮿ڴ�ӡ��Ϣ�����·�תled���
    *   3   ex����PA0���½��غ������ذ�ͬһ���ص�������x++���Ӵ������
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32F4 GPIO_EXTI example"
#define EXAMPLE_DATE	"2017-09-10"

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    uart1.flush();
    LED1.mode(OUTPUT_PP);
    int cnt = 10;
    
}

int main(void)
{
    setup();
    while(1)
    {
        LED1.toggle();
        delay(1000);
    }
}
