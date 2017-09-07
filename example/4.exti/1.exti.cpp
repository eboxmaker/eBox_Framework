 /**
  ******************************************************************************
  * @file    main.cpp
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


uint32_t xx;

//创建一个外部中断对象，链接PA8，中断触发模式为下降沿触发
Exti ex(&PA8, FALLING);

void exit()//外部中断事件
{
    xx++;
    uart1.printf("\r\nxx = %d", xx);
    PB8.toggle();
}
void setup()
{
    ebox_init();
    uart1.begin(115200);
    PB8.mode(OUTPUT_PP);
    ex.begin();//初始化外部中断
    ex.attach(exit);//绑定外部中断触发事件
    ex.interrupt(ENABLE);//使能外部中断
}
int main(void)
{
    setup();
    while(1)
    {
        ;
    }
}