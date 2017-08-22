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
#include "../normal/filters.h"


LpfRc lpfrc(20,1000);
FilterAverage filter_average(50);
FilterMoveWindow filter_window(100);
FilterMidAverage filter_mid(5);
FilterDelta filter_delta(10);
#define LEN 2*1000



FirFilter fir(49,20,1000);



#define BANDWIDTH_HZ          45     // 3-dB bandwidth of the filter
                                      // The LPF tracks the sample time internally
LPF lpf3(BANDWIDTH_HZ,IS_BANDWIDTH_HZ,3);





// return the current time
float time() {
  return float( millis() ) * 1e-3;
}


Timer timer2(TIM2);
int8_t in_signal[LEN];
int8_t out_signal[LEN];
float out_signal2[LEN];
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
    {
//        if(i % 3 == 0)
//            in_signal[i] = 50.0*sin( 2*PI * i * 1e-3) +  random(-10,10) ;
//        else
            in_signal[i] = 50.0*sin( 2*PI * i * 1e-3) +  20.0*sin( 2*PI * 50 * i * 1e-3)  ;
    }
    
    
    //quick_sort(xxx,0,4);

}


int main(void)
{
    static uint64_t last_time = millis();
    static uint64_t last_time1 = millis();
    float xxx[] = {2.1,1.5,4.89,6.23,3.45};
    setup();

    uint64_t start,end;
    while(1)
    {
        start = millis();
        for(int i = 0; i < LEN; i++)
            out_signal[i] = fir.in(in_signal[i]);
        end = millis();

        uart1.printf("FIR = %d\r\n",end - start);
        
        start = millis();
        for(int i = 0; i < LEN; i++)
            out_signal2[i] = lpf3.NextValue(in_signal[i]);
        end = millis();
        uart1.printf("LPF = %d\r\n",end - start); 
        
        int j = 0;
        for(int i = 0; i < LEN; i++)
        {
            uart1.printf("%d\t",in_signal[i]);   
            
//            in_signal[i] = filter_delta.in(in_signal[i]);
//            uart1.print(in_signal[i]); uart1.print("\t"); 

            
            out_signal[i] = fir.in(in_signal[i]);
            uart1.printf("%d\t",out_signal[i]);   
            
            

//            if(filter_average.sample(in_signal[i]))
//            {
//                    out_signal2[j] = filter_average.out();
//                    uart1.print(out_signal2[j]); 

//            }   
//            else
//            {
//                uart1.print(out_signal2[j]); 
//                uart1.print("\t"); 

//            }
            //out_signal2[i] = lpfrc.in(out_signal2[i]);
            
//            if(filter_mid.sample(in_signal[i]))
//            {
//                    out_signal2[i] = filter_mid.out();
//                    out_signal2[i+1] = filter_mid.out();
//                    out_signal2[i+2] = filter_mid.out();
//            }   
            out_signal2[i] = lpfrc.in(in_signal[i]);
            uart1.printf("%0.2f\t",out_signal2[i]);   
            uart1.printf("\r\n");
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




