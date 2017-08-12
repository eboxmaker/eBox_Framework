/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */


#include "ebox.h"
Timer timer1(TIM1);

void t2it()
{
        PB8.reset();
        delay_us(10);
        PB8.set();
        delay_us(10);
    
        uart1.printf("111");
        delay_ms(100);
        uart1.printf("222");
        delay_ms(100);

}

void setup()
{
    ebox_init();
    uart1.begin(115200);
    PB8.mode(OUTPUT_PP);
    
    timer1.begin(1);
    timer1.attach(t2it);
    timer1.interrupt(ENABLE,0,0);
    timer1.start();
    
}
int main(void)
{
    setup();
    //no_interrupts();
    while(1)
    {

        delay_ms(1000);

   
    }

}


