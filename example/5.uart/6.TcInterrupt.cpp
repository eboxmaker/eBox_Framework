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

#include "ebox.h"
#include "bsp_ebox.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"UART transmit complete example"
#define EXAMPLE_DATE	"2018-08-08"

uint8_t count;
void tc_evnet()
{
    count++;
    LED1.toggle();
}
void setup()
{
    ebox_init();
    uart1.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

    uart1.attach(tc_evnet,TcIrq);
    uart1.interrupt(TcIrq,ENABLE);
    PB8.mode(OUTPUT_PP);
    PB8.reset();
	no_interrupts();
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




