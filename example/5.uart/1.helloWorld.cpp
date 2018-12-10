/**
 ******************************************************************************
 * @file    main.cpp
 * @author  shentq
 * @version V1.2
 * @date    2016/08/14
 * @brief   ebox application example .
*					 2018-8-5	通过引入bsp，定义硬件端口，方便例程可以在不同平台上运行
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
	*	1	通过串口打印消息
	*/
/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"hello world example"
#define EXAMPLE_DATE	"2018-08-06"

void setup()
{
    ebox_init();
    LED1.mode(OUTPUT_PP);
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
}
int main(void)
{
    setup();
    while(1)
    {
        UART.printf("hello World !\r\n");
        delay_ms(1000);
        LED1.set();
        delay_ms(1000);
        LED1.reset();
    }
}




