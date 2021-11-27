/**
 ******************************************************************************
 * @file    main.cpp
 * @author  cat_li
 * @version V1.0
 * @date    2017/07/13
 * @brief   ebox exti example, 基于stm32nucleo(072)平台验证
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
	*	1	此例程演示了GPIO中断
    *   2   其中userbt1连接用户按键，按下和弹起绑定不同的回调函数
            弹起串口打印信息，按下反转led输出
    *   3   ex连接PA0，下降沿和上升沿绑定同一个回调函数，x++并从串口输出
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"STM32F0 GPIO_EXTI example"
#define EXAMPLE_DATE	"2017-09-10"



void setup()
{
    ebox_init();

}


int main(void)
{
    setup();
    while(1)
    {
        //userbt1.soft_triger();
        delay_ms(1000);
    }
}




