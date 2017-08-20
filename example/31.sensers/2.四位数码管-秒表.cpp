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
#include "button.h"
#include "TM1637.h"

#define ON 1
#define OFF 0

Timer timer2(TIM2);
Button  btn(&PA8, 1);
TM1637 tm(&PA4,&PA3);


int8_t TimeDisp[4];
uint8_t Update = ON;
uint8_t time_flag = ON;
uint8_t millis_10 = 0;
uint8_t second = 0;

void t2_event()
{
    millis_10++;
    if(millis_10 == 100)
    {
        second++;
        if(second == 100)
            second = 0;
        millis_10 = 0;
    }
  Update = ON;

}
void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("\r\nuart1 115200 ok!\r\n");
    tm.begin();
    tm.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
    btn.begin();
    timer2.begin(100);
    timer2.attach(t2_event);
    timer2.interrupt(ENABLE);
    timer2.start();

}

void TimeUpdate(void)
{
  TimeDisp[2] = millis_10 / 10;
  TimeDisp[3] = millis_10 % 10;
  TimeDisp[0] = second / 10;
  TimeDisp[1] = second % 10;
  Update = OFF;
}

int main(void)
{
    uint64_t start,end;

    setup();

    while(1)
    {
        btn.loop();
        if(btn.release())
        {
            if(time_flag == ON)
            {
                timer2.stop();
                time_flag = OFF;
            }
            else
            {
                timer2.start();
                time_flag = ON;
            }
                
        }
        if(btn.pressed_for(1000,1))
        {
            second = 0; 
            millis_10 = 0;
            time_flag = OFF;
            timer2.stop();
            TimeUpdate();
            tm.display(TimeDisp);
        }
        if(Update == ON)
        {
            TimeUpdate();
            tm.display(TimeDisp);
        }
    }
}




