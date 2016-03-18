/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "ucos_ii.h"
static OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];		  //定义栈

void xx()
{
    OSIntEnter();
    OSTimeTick();
    OSIntExit();
}

void setup()
{
    ebox_init();
    PB8.mode(OUTPUT_PP);
    set_systick_user_event_per_sec(OS_TICKS_PER_SEC);
    attch_systick_user_event(xx);
}
void Task_LED(void *p_arg)
{
    (void)p_arg;                		// 'p_arg' 并没有用到，防止编译器提示警告
    while (1)
    {
        PB8.toggle();
        //        delay_ms(500);
        OSTimeDlyHMSM(0, 0, 0, 500);
    }
}
int main(void)
{
    setup();
    OSInit();
    OSTaskCreate(Task_LED, (void *)0,
                 &startup_task_stk[STARTUP_TASK_STK_SIZE - 1], STARTUP_TASK_PRIO);

    OSStart();
    return 0;


}


