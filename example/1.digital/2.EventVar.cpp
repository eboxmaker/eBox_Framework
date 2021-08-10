/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
	*					 2018-8-2	通过引入bsp，定义硬件端口，方便例程可以在不同平台上运行
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */


#include "ebox.h"
#include "bsp_ebox.h"
#include "EventVar.h"

/**
	*	1	此例程需要调用apps目录下的EventGpio , eventVar,EventManager模块
	*	2	此例程演示了IO事件响应，分别为低电平，高电平，下降沿，上升沿，单击，释放，长按
	*	3	高定平，低电平会连续触发，触发周期5ms，可通过修改EvenGpio.h中的IO_EDGE_FILTER_COUNTS修改
	* 4	长按时会禁用单击事件，且长按发生后不触发释放事件。但不影响上升沿和下降沿事件
	* 5 如果没有长按，则下降沿=单击，上升沿=释放
	*
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"EventVar example"
#define EXAMPLE_DATE	"2019-05-17"
/** 创建EventGpio对象，并挂载事件回调函数高电平，低电平，上升沿，
  *下降沿，单击，释放，长按.不需要响应的事件不需要处理
	*/
// 使用长按事件会自动禁用单击事件，且长按发生后不触发释放事件。但不影响上升沿和下降沿事件
// 使用长按事件，可以和释放事件配合，当没有触发长按事件的时候，释放事件会被执行

uint8_t volume = 0;
EventVarUint8 var(&volume,"volume");

void up(Object *sender)
{
    EventVarUint8 *ptr = (EventVarUint8 *)sender; 
    UART.print("var:");
    UART.print((uint8_t)(*ptr->var));
    UART.println(" 增加");
}
void down(Object *sender)
{
    EventVarUint8 *ptr = (EventVarUint8 *)sender; 
    UART.print("var:");
    UART.print((uint8_t)(*ptr->var));
    UART.println(" 减少");
}void change(Object *sender)
{
    UART.println("var 变化");
}

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    
    var.event_changed = change;
    var.event_nag_edge = down;
    var.event_pos_edge = up;
    
    manager.add(&var);
    manager.print_list(UART);
}
uint32_t last;
uint8_t flag = 0;
int main(void)
{
    setup();

    while(1)
    {
        manager.loop();
        delay_ms(1);
        if(millis() - last > 1000)
        {
            last = millis();
            if(flag==0)
                volume++;
            else
                volume--;
            if(volume >= 3)
                flag = 1;
            else if (volume == 0)
                flag = 0;
        }
        
    }
}
