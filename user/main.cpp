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
    LED2.mode(OUTPUT_PP);
    LED3.mode(OUTPUT_PP);
    LED1.set();
    no_interrupts();
}
int main(void)
{
    setup();
    while(1)
    {
//        LED1.set();
//        LED1.reset();
//        GPIO_SetBits(GPIOB, GPIO_Pin_8);
//        GPIO_ResetBits(GPIOB, GPIO_Pin_8);
          GPIOI->BSRRH = GPIO_Pin_6;
          GPIOI->BSRRL = GPIO_Pin_6;     


    }
}


