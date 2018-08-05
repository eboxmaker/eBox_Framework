 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   ebox application example .
	*					 2018-8-5	通过引入bsp，定义硬件端口，方便例程可以在不同平台上运行 
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

/**
	*	1	通过串口打印消息
	*/

void setup()
{
    ebox_init();
    UART.begin(115200);
}
int main(void)
{
    setup();
    while(1)
    {
        UART.printf("hello World !\r\n");
        delay_ms(1000);
    }
}




