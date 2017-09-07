/**
  ******************************************************************************
  * @file    pwm.cpp
  * @author  shentq
  * @version V1.2
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


/* Includes ------------------------------------------------------------------*/


#include "ebox.h"



void setup()
{
    ebox_init();
    uart1.begin(115200);
    PB9.mode(OUTPUT_PP);
}
int main(void)
{
    setup();
    uart1.printf("core:%d\r\n",cpu.clock.core);
    uart1.printf("core:%d\r\n",cpu.clock.core);
    uart1.printf("hclk:%d\r\n",cpu.clock.hclk);
    uart1.printf("pclk1:%d\r\n",cpu.clock.pclk1);
    uart1.printf("pclk2:%d\r\n",cpu.clock.pclk2);
    uart1.printf("cpu id:0x%x %x %x\r\n",cpu.chip_id[0],cpu.chip_id[1],cpu.chip_id[2]);
    uart1.printf("size:%dK\r\n",cpu.flash_size);
    uart1.printf("company:%s\r\n",cpu.company);
    uart1.printf("company:%dK times add per second\r\n",cpu.ability/1000);

    while(1)
    {
        PB9.toggle();
        delay_ms(200);
    }
}




