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
#include "FirFilter.h"



#define LEN 1*1000
int16_t in_signal[LEN];
int16_t out_signal[LEN];

FirFilter fir;


void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("\r\nuart1 115200 ok!\r\n");
    
    float beta;

    int kaiserWindowLength;
	float sampFreq = 1000;
    
	// Low and high pass filters
	float transFreq1 = 20;
	float transFreq2 = 70;



    //设定滤波器特性，0.01：文波系数；10： transition band width；sampFreq：采样率
    //该函数会根据前两个值得设定，计算出滤波器阶数。
	fir.calculateKaiserParams(0.01, 10, sampFreq, &kaiserWindowLength, &beta);
//	float *lpf = fir.create1TransSinc(kaiserWindowLength, transFreq1, sampFreq, LOW_PASS);
	float *lpf = fir.create1TransSinc(kaiserWindowLength, transFreq1, sampFreq, HIGH_PASS);
//	float *lpf = fir.create2TransSinc(kaiserWindowLength, transFreq1,transFreq2, sampFreq, BAND_PASS);
//	float *lpf = fir.create2TransSinc(kaiserWindowLength, transFreq1,transFreq2, sampFreq, BAND_STOP);
	float *lpf_kaiser = fir.createKaiserWindow(lpf, NULL, kaiserWindowLength, beta);
    uart1.printf("kaiserWindowLength = %d\r\n",kaiserWindowLength);
    uart1.printf("beta = %d\r\n",beta);
    fir.set(lpf_kaiser,kaiserWindowLength);
    for(int i = 0; i < LEN;i++)
        in_signal[i] =50 + 10.0*sin( 2*PI *50* i * 1e-3) + 10.0*sin( 2*PI *10* i * 1e-3)   ;

    uart1.printf("mem : %d\r\n",ebox_get_free());
}


int main(void)
{
    uint64_t start,end;

    setup();

    start = micros();
    for(int i = 0; i < LEN; i++)
        out_signal[i] = fir.in(in_signal[i]);
    end = micros();
    uart1.printf("micros per calculation = %0.1fus\r\n",(end - start)/1000.0);
    for(int i = 0; i < LEN; i++)
    {
        out_signal[i] = fir.in(in_signal[i]);
        uart1.printf("%d\t",in_signal[i]);   
        uart1.printf("%d\t",out_signal[i]);   
        uart1.println();
    }
    while(1)
    {



    }
}




