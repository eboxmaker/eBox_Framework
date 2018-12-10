/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : cat_li
  * @version: V2.0
  * @date   : 2017/12/17
  * @brief   ebox application example .
  *	core API中关于systick中断回调应用，。
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */


#include "ebox.h"
#include "bsp_ebox.h"

/**
	*	1	此例程演示了systemtick中断回调的应用
    *   2   回调函数SystickCallBack每1ms 或 N ms 调用一次
	*/

#define N     100		//调用周期,systick为1ms,所以调用周期为10ms

/**
	*	1	通过串口打印消息
	*/
/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"systemTick example"
#define EXAMPLE_DATE	"2018-10-01"

//    回调函数，调用周期取决于systick中断回调注册函数。如果systick中断只处理一个任务，使用
void SystickCallBack(void)
{
    LED1.toggle();
}

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    LED1.mode(OUTPUT_PP);

    // 默认周期回调。1ms
    attachSystickCallBack(SystickCallBack);
    // 指定回调周期，回调周期是系统默认周期的N倍,500ms
    UART.printf("单任务处理,每%dms执行一次(led 反转)", N);
    //  attachSystickCallBack(SystickCallBack,N);
}

int main(void)
{
    setup();
    while (1)
    {
    }
}
