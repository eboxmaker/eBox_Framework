/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
	*					 2018-7-31	通过引入bsp，定义硬件端口，方便例程可以在不同平台上运行
  *
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */


#include "ebox.h"
#include "bsp_ebox.h"


void setup()
{
    ebox_init();
    LED1.mode(OUTPUT_PP);
}
int main(void)
{
    setup();
    while(1)
    {
        LED1.set();
        delay_ms(500);
        LED1.reset();
        delay_ms(500);
    }
}


