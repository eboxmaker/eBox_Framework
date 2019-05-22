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
#include "EventGpio.h"
#include "EventVar.h"
#include "ringbuf.h"
#include "bsp_ebox.h"

/**
	*	1	此例程需要调用apps目录下的EventGpio , eventVar,EventManager模块
	*	2	此例程演示了IO事件响应，分别为低电平，高电平，下降沿，上升沿，单击，释放，长按
	*	3	高定平，低电平会连续触发，触发周期5ms，可通过修改EvenGpio.h中的IO_EDGE_FILTER_COUNTS修改
	* 4	长按时会禁用单击事件，且长按发生后不触发释放事件。但不影响上升沿和下降沿事件
	* 5 如果没有长按，则下降沿=单击，上升沿=释放
	*
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"EventGPIO&EventVar example"
#define EXAMPLE_DATE	"2019-05-17"
/** 创建EventGpio对象，并挂载事件回调函数高电平，低电平，上升沿，
  *下降沿，单击，释放，长按.不需要响应的事件不需要处理
	*/
// 使用长按事件会自动禁用单击事件，且长按发生后不触发释放事件。但不影响上升沿和下降沿事件
// 使用长按事件，可以和释放事件配合，当没有触发长按事件的时候，释放事件会被执行
EventGpio btn(&PA8, 1,"btn");

uint8_t volume = 0;
EventVarUint8 var(&volume,"volume");

// 下降沿检测
void neg(Object *sender)
{
    UART.println("检测到下降沿信号");
}
// 上升沿检测
void pos(Object *sender)
{
    UART.println("检测到上升沿信号");
}
// 高电平回调函数
void high(Object *sender)
{
    UART.println("检测到高电平");
}

// 高电平回调函数
void low(Object *sender)
{
    UART.println("检测到低电平");
}
// 单击回调函数
void click(Object *sender)
{
    UART.println("检测到按键单击");
}
// 释放回调函数
void release(Object *sender)
{
    if(&btn == sender)
        UART.println("检测到按键释放");
    EventGpio *newObject = (EventGpio *)sender;
    UART.println(newObject->name);
    UART.println(sender->name);}
// 长按回调函数
void long_press(Object *sender)
{
    UART.println("检测到长按，长按时不响应单击");
}
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


EventManager manager;
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    //event_io_1.begin(1);
    
    
    btn.event_click = click;
    btn.event_release = release;
    btn.event_long_press = long_press;
    btn.long_press_type = EventGpio::Continue;
//    btn.event_high = high;
//    btn.event_low = low;
//    btn.event_neg_edge = neg;
//    btn.event_pos_edge = pos;
    
    var.event_changed = change;
    var.event_nag_edge = down;
    var.event_pos_edge = up;
    btn.begin();
    
    manager.add(&btn);
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
