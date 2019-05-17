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
	*	1	此例程需要调用apps目录下的EventVar.h , EventManager模块
	*	2	此例程演示了Var事件响应，分别为变大，变小，变化三种事件
	*
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"EventVar example"
#define EXAMPLE_DATE	"2019-05-17"

void up()
{
    UART.println("var 增加");
}
void down()
{
    UART.println("var 减少");
}void change()
{
    UART.println("var 变化");
}

uint8_t volume = 0;
EventVar<uint8_t> var(&volume);

EventManager event_manager;
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);    
   
   
    var.event_changed = change;
    var.event_nag_edge = down;
    var.event_pos_edge = up;
    
    event_manager.add(&var);
}
uint32_t last;
uint8_t flag = 0;
int main(void)
{
    setup();

    while(1)
    {
        event_manager.loop();
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
