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

Exti ex(&PA8, EXTI_Trigger_Falling);

void exit()
{
    xx++;
    uart1.printf("\r\nxx = %d", xx);
}


class Test 
{
    public:
    void event() 
    {
        PB8.toggle();
    }
};
Test test;

void setup()
{
    ebox_init();
    uart1.begin(115200);
    PB8.mode(OUTPUT_PP);
    ex.begin();
//    ex.attach(exit);
    ex.attach(&test,&Test::event);
    ex.interrupt(ENABLE);
}


int main(void)
{
    setup();
    while(1)
    {
        ;
    }


}




