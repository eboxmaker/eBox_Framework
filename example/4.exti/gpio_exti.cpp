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
#include "ebox_exti.h"
/**
	*	1	此例程演示了GPIO中断
    *   2   其中userbt1连接用户按键，按下和弹起绑定不同的回调函数
            弹起串口打印信息，按下反转led输出
    *   3   ex连接PA0，下降沿和上升沿绑定同一个回调函数，x++并从串口输出
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"STM32F0 GPIO_EXTI example"
#define EXAMPLE_DATE	"2017-09-10"


uint32_t xx;


Exti   userbt1(&BTN1);
//Exti   ex(&BtnPin);

/**
 *@brief    静态回调函数
 *@param    none
 *@retval   none
*/
void fall()
{
    xx--;
    UART.printf("\r\n falling,xx = %d", xx);
}

void rise()
{
    xx++;
    LED1.toggle();
    UART.printf("\r\n rising, xx = %d", xx);
}

void exti_event()
{
    xx++;
    LED1.toggle();
    if(userbt1.read())
        UART.printf("\r\n rise, xx = %d", xx);
    else
        UART.printf("\r\n fall, xx = %d", xx);
}

/**
 *@brief    测试类，用来测试类成员绑定
 *@param    none
 *@retval   none
*/
class Test
{
public:
    void event()
    {
        LED1.toggle();
    }
};
Test test;

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    LED1.mode(OUTPUT_PP);
    // 上升沿，下降沿均触发,绑定同一个中断回调函数
    userbt1.begin();
    userbt1.attach(exti_event);
    userbt1.interrupt(ApiExti::TrigFall,ENABLE);

//    // 上升沿，下降沿调用不同的回调函数
//    userbt1.begin();
//    userbt1.attach(rise);
//    userbt1.interrupt(TrigFall,ENABLE);

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




