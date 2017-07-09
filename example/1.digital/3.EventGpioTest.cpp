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
#include "EventGpio.h"

void neg()
{
    uart1.println("neg");
}
void pos()
{
    uart1.println("pos");
}
void high()
{
    uart1.println("high");
}
void click()
{
    uart1.println("click");
}
void release()
{
    uart1.println("release");
}
void long_press()
{
    uart1.println("long");
}
void click1()
{
    uart1.println("click1");
}
void release1()
{
    uart1.println("release1");
}
void long_press1()
{
    uart1.println("long1");
}
EventGpio event_io_1(&PA8,0,0,0,0,click,release,0);
EventGpio event_io_2(&PB1,0,0,0,neg,click1,release1,long_press1);
EventManager io_manager;
void setup()
{
    ebox_init();
    uart1.begin(115200);
    event_io_1.begin(1);
    event_io_2.begin(1);
    io_manager.add(&event_io_1);
    io_manager.add(&event_io_2);
}
int main(void)
{
    setup();

    while(1)
    {
        io_manager.process();
        delay_ms(1);
    }
}



