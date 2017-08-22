/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2017/8/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "math.h"
#include "BMP280-3.3.h"
#include "TM1637.h"

#define ON 1
#define OFF 0

Timer timer2(TIM2);
TM1637 tm(&PA4,&PA3);


int8_t TimeDisp[4];
DateTime_t dt;
uint8_t halfsecond = 0;
uint8_t clock_point = 0;
unsigned char Update;

void t2_event()
{
    halfsecond++;
    if(halfsecond == 2)
    {
        dt.sec ++;
        if(dt.sec == 60)
        {
          dt.min ++;
          if(dt.min == 60)
          {
            dt.hour ++;
            if(dt.hour == 24)dt.hour = 0;
            dt.min = 0;
          }
          dt.sec = 0;
        }    
        halfsecond = 0;
    }
  clock_point = (~clock_point) & 0x01;
  Update = ON;

}
void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("\r\nuart1 115200 ok!\r\n");
    tm.begin();
    tm.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
    
    timer2.begin(2);
    timer2.attach(t2_event);
    timer2.interrupt(ENABLE);
    timer2.start();

}

void TimeUpdate(void)
{
  if(clock_point)tm.point(POINT_ON);
  else tm.point(POINT_OFF); 
  TimeDisp[0] = dt.hour / 10;
  TimeDisp[1] = dt.hour % 10;
  TimeDisp[2] = dt.min / 10;
  TimeDisp[3] = dt.min % 10;
  Update = OFF;
}

int main(void)
{
    uint64_t start,end;

    setup();

    while(1)
    {
        if(Update == ON)
        {
            TimeUpdate();
            tm.display(TimeDisp);
        }
    }
}




