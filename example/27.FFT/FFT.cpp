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
#include "arm_math.h"
void fft_test();
float32_t testIn[2048];

#define TEST_LENGTH_SAMPLES 2048 
 
/* ------------------------------------------------------------------- 
* External Input and Output buffer Declarations for FFT Bin Example 
* ------------------------------------------------------------------- */ 
extern float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES]; 
static float32_t testOutput[TEST_LENGTH_SAMPLES/2]; 
 
/* ------------------------------------------------------------------ 
* Global variables for FFT Bin Example 
* ------------------------------------------------------------------- */ 
uint32_t fftSize = 1024; 
uint32_t ifftFlag = 0; 
uint32_t doBitReverse = 1; 
 
/* Reference index at which max energy of bin ocuurs */ 
uint32_t refIndex = 213, testIndex = 0; 
 
/* ---------------------------------------------------------------------- 
* Max magnitude FFT Bin test 
* ------------------------------------------------------------------- */ 

void setup()
{
    ebox_init();
    uart1.begin(115200);

}
uint32_t time1,time2;

int main(void)
{
    setup();

    
    for(int i = 0 ; i < 2048 ; i++)
    {
        testIn[i] = sin(i/10.0+(1.5*PI));
       // uart1.printf("%1.3f\r\n",testIn[i]);
    }
    
    while(1)
    {


        time1 = millis();
        fft_test();
        time2 = millis();
        for(int i = 0; i < 1024; i++)
            uart1.printf("%3.2f\r\n",testOutput[i]);
        uart1.printf("\r\nFFT:(time=%dms)",time2 - time1);


        while(1)
        delay_ms(1000);
    }
}
 
void fft_test()
{
	arm_status status; 
	arm_cfft_radix4_instance_f32 S; 
	float32_t maxValue; 
	 
	status = ARM_MATH_SUCCESS; 
	 
	/* Initialize the CFFT/CIFFT module */  
	status = arm_cfft_radix4_init_f32(&S, fftSize,  
	  								ifftFlag, doBitReverse); 
	 
	/* Process the data through the CFFT/CIFFT module */ 
	arm_cfft_radix4_f32(&S, testIn); 
	 
	 
	/* Process the data through the Complex Magnitude Module for  
	calculating the magnitude at each bin */ 
	arm_cmplx_mag_f32(testIn, testOutput,  
	  				fftSize);  
	 
	/* Calculates maxValue and returns corresponding BIN value */ 
	arm_max_f32(testOutput, fftSize, &maxValue, &testIndex); 
	 
//	if(testIndex !=  refIndex) 
//	{ 
//		status = ARM_MATH_TEST_FAILURE; 
//	} 
}


