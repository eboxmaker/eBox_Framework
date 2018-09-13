 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  shentq
  * @version V2.0
  * @date    2016/08/14
  * @brief   ebox application example .
  *		 2018-8-6	通过引入bsp，定义硬件端口，方便例程可以在不同平台上运行
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

/**
    *	1	此例程演示串口的发送&接收中断
	*   2	收到数据后会同时将数据返回，类似于超级终端里的回显效果
	*	3	如果不能实现效果，请切换发送格式，建议不要使用hex格式
	*/
#include "ebox.h"
#include "bsp_ebox.h"


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"send&recv_interrupt example"
#define EXAMPLE_DATE	"2018-08-06"

uint8_t count;
// 接受中断回调
void rx_event()
{
    uint8_t c;
    c = UART.read();
    UART.write(c);
	LED1.toggle();
}
// 发送中断回调，翻转LED状态
void tc_evnet()
{
    count++;
    LED2.toggle();
}
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
    // 绑定响应中断，并使能
    UART.attach(rx_event,RxIrq);
    UART.attach(tc_evnet,TcIrq);
    UART.interrupt(RxIrq,ENABLE);
    UART.interrupt(TcIrq,ENABLE);
    
    LED1.mode(OUTPUT_PP);
    LED1.reset();
    LED2.mode(OUTPUT_PP);
    LED2.reset();
}

int main(void)
{
    setup();
    while(1)
    {
        //uart1.printf("uart is ok ! count = %d\r\n", count);
        delay_ms(1000);
    }
}



