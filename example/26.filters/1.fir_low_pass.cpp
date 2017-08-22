/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "math.h"
#include "lpf.h"


#include "FirFilter.h"
#define LEN 2*1000
FirFilter fir(55,45,1000);
// return the current time
float time() {
  return float( millis() ) * 1e-3;
}


Timer timer2(TIM2);
int8_t in_signal[LEN];
int8_t out_signal[LEN];
int count = 0;
void test()
{
    if(count < LEN)
    {
    out_signal[count] = fir.in(in_signal[count]);
    uart1.print(in_signal[count]);   uart1.print("\t");
    uart1.print(out_signal[count]);   uart1.print("\t");
    uart1.println();
    }
    count++;

}
void t2it()
{
    PB8.toggle();  
}
void setup()
{
    double Answer[5], SquarePoor[4];
    ebox_init();
    uart1.begin(115200);
    ///uart1.printf("\r\nuart1 115200 ok!\r\n");
    PB1.mode(AIN);
    timer2.begin(1000);
    timer2.attach(test);
    //timer2.interrupt(ENABLE);
    timer2.start();
    for(int i = 0; i < LEN;i++)
    in_signal[i] = 50.0*sin( 2*PI * i * 1e-3) + 10.0*sin( 2*PI *50* i * 1e-3) + + 20.0*sin( 2*PI *200* i * 1e-3)  ;

}


int main(void)
{
    static uint64_t last_time = millis();
    static uint64_t last_time1 = millis();
    setup();

    uint64_t start,end;
    while(1)
    {
        start = millis();
        for(int i = 0; i < LEN; i++)
            out_signal[i] = fir.in(in_signal[i]);
        end = millis();
        uart1.printf("micro s = %d\r\n",start);
        uart1.printf("micro e = %d\r\n",end);
        uart1.printf("micro d = %d\r\n",end - start);
        for(int i = 0; i < LEN; i++)
        {
            out_signal[i] = fir.in(in_signal[i]);
            uart1.print(in_signal[i]);   uart1.print("\t");
            uart1.print(out_signal[i]);   uart1.print("\t");
            uart1.println();
        }
        while(1);

//        if(millis() - last_time > 1)
//        {
//            last_time = millis();

//        }
//        if(millis() - last_time1 > 20)
//        {
//            last_time1 = millis();
//        }                    

        //delay_ms(100);
    }
}




