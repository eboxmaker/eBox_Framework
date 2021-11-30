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
#include "rtthread.h"
#include "cxx_Thread.h"

using namespace rtthread;

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
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    LED1.mode(OUTPUT_PP);
}


//动态线程任务
static void dynamic_entry(void *param)
{
    static int cnt = 0;
	
    while (++cnt)
    {
		rt_kprintf("dynamic_thread is run:%d\n",cnt);
        rt_thread_mdelay(500);
    }
}
int thread_sample(void)
{
    static rt_thread_t thread_id = RT_NULL;
    thread_id = rt_thread_create("dynamic_th",    //名称
                                 dynamic_entry,   //线程代码
                                 RT_NULL,         //参数
                                 1024,            //栈大小
                                 15,              //优先级
                                 20);             //时间片

    if (thread_id != RT_NULL)
        rt_thread_startup(thread_id);			  //线程进入就绪态
    else
        rt_kprintf("dynamic_thread create failure\n");
					//线程进入就绪态		
									 
    return RT_EOK;									 
}
void myTask(void *p)
{
    while(1)
    {
        uart1.printf("MyTask Run\n");
        Thread::sleep(1000);
    }
}

int main(void)
{
    setup();
    thread_sample();
    Thread task1(myTask);
    task1.start();
    while(1)
    {
        LED1.toggle();
        rt_thread_mdelay(1000);
        
    }
}
