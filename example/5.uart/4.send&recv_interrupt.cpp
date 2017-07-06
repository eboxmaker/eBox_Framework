 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  shentq
  * @version V2.0
  * @date    2016/08/14
  * @brief   ebox application example .
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

/*
一个简单的命令帧接收示例
*/
#include "ebox.h"
u8 count;
void rx_event()
{
    uint8_t c;
    c = uart1.read();
    uart1.write(c);
}
void tc_evnet()
{
    count++;
    PB8.toggle();
}
void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.attach(rx_event,RxIrq);
    uart1.attach(tc_evnet,TcIrq);
    uart1.interrupt(RxIrq,ENABLE);
    uart1.interrupt(TcIrq,ENABLE);
    PB8.mode(OUTPUT_PP);
    PB8.reset();
}

int main(void)
{
    setup();
    while(1)
    {
        uart1.printf("uart is ok ! count = %d\r\n", count);
        delay_ms(1000);
    }
}




