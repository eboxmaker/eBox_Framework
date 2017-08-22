/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "../common/apps/pid/pid_v1.h"
#include "math.h"
#include <Filters.h>
#include <biquadfilter.h>
#include "lpf.h"
int FreqCutoff = 8;
int SampleRate = 100;

BiQuadFilter filterObject(FreqCutoff, SampleRate);


#define BANDWIDTH_HZ          1.0     // 3-dB bandwidth of the filter
#define SAMPLE_TIME_SEC       0.001   // How often are we upodating the loop?
                                      // The LPF tracks the sample time internally
#define SIGNAL_FREQUENCY_HZ   0.3     // Our test input signal frequency
LPF lpf(BANDWIDTH_HZ,IS_BANDWIDTH_HZ);      
LPF lpf3(BANDWIDTH_HZ,IS_BANDWIDTH_HZ,3);


float lowFrequency = 10;                     // test signal frequency (Hz)
float highFrequency = 80;                     // test signal frequency (Hz)
float testAmplitude = 1;                   // test signal amplitude
float testOffset = 1;

float windowLength = 20.0/lowFrequency;     // how long to average the signal, for statistist

float testSignalSigma = testAmplitude / sqrt(2.0);         // the RMS amplitude of the test signal
float testSignal3dbSigma = testSignalSigma / sqrt(2.0);    // the RMS amplitude of the test signal, down -3db

float printPeriod = 5.0;
float inputValue;

// return the current time
float time() {
  return float( micros() ) * 1e-6;
}
void testOnePoleFilters() {
  // filters are test with a sine wave input, keep track of those values here for a sanity check
  RunningStatistics inputStats;                 // create statistics to look at the raw test signal
  inputStats.setWindowSecs( windowLength );
  
  FilterOnePole filterOneLowpass( LOWPASS, 5 );   // create a one pole (RC) lowpass filter
  RunningStatistics filterOneLowpassStats;                    // create running statistics to smooth these values
  filterOneLowpassStats.setWindowSecs( windowLength );
  
  FilterOnePole filterOneHighpass( HIGHPASS, 30 );  // create a one pole (RC) highpass filter
  RunningStatistics filterOneHighpassStats;                    // create running statistics to smooth these values
  filterOneHighpassStats.setWindowSecs( windowLength );
  
  float startTime = time();
  float nextPrintTime = time();
//  while( true ) {
    // update all real time classes
      


    // update the test value statistics
//    inputStats.input( inputValue);
//    
//    // update the one pole lowpass filter, and statistics
//    filterOneLowpass.input( inputValue );
//    filterOneLowpassStats.input( filterOneLowpass.output() );
//    
//    // update the one pole highpass filter, and statistics
//    filterOneHighpass.input( inputValue );
//    filterOneHighpassStats.input( filterOneHighpass.output() );
   // uart1.printf("%0.2f\t%0.2f\t%0.2f\r\n",inputValue,filterOneLowpass.output(),filterOneHighpass.output());

//    if( time() > nextPrintTime ) {
//      // display current values to the screen
//      nextPrintTime += printPeriod;   // update the next print time
//      
//      uart1.print( "\n" );
//      uart1.print( "time: " ); uart1.print( time() );
//      
//      // output values associated with the inputValue itsel
//      uart1.print( "\tin: " ); uart1.print( inputStats.mean() ); uart1.print( " +/- " ); uart1.print( inputStats.sigma() );
//      uart1.print( " (" ); uart1.print( testOffset ); uart1.print( " +/- " ); uart1.print( testSignalSigma ); uart1.print( ")" );
//      
//      // output values associated with the lowpassed value
//      uart1.print( "\tLP1: " ); uart1.print( filterOneLowpassStats.mean() ); uart1.print( " +/- " ); uart1.print( filterOneLowpassStats.sigma() );
//      uart1.print( " (" ); uart1.print( testOffset ); uart1.print( " +/- " ); uart1.print( testSignal3dbSigma ); uart1.print( ")" );

//      // output values associated with the highpass value
//      uart1.print( "\tHP1: " ); uart1.print( filterOneHighpassStats.mean() ); uart1.print( " +/- " ); uart1.print( filterOneHighpassStats.sigma() );
//      uart1.print( " (" ); uart1.print( "0.0" ); uart1.print( " +/- " ); uart1.print( testSignal3dbSigma ); uart1.print( ")" );
//    }
//  }
}

void PlotSineWaveWithNoiseResponse ( void ) {

    static unsigned long timer = millis();
    static unsigned int dt = 0;
    float inputSignal, twoPiDt = 2.0 * PI * dt * SAMPLE_TIME_SEC;

    inputSignal = 5.0 * sin(SIGNAL_FREQUENCY_HZ * twoPiDt) + 
                  /* now add some higher order signals to filter out */
                  2.0 * sin(BANDWIDTH_HZ * 10 * twoPiDt) +
                  1.0 * sin(BANDWIDTH_HZ * 20 * twoPiDt) ;//+
                 // /* some 'noise' */ (float)random(-100, 100) / 100.0;
    dt++;
    inputSignal = 5.0*sin( 2*PI * time()) + 2.0*sin( 2*PI *20* time())  ;

    /*
     * Demonstrate use of Reset()
     */
    if ( (millis() - timer) >= 5000 ) {
        lpf3.Reset(1.0);   // Reset lpf3 to an initalValue of 1.0
        timer = millis();
    }

    /* 
     *  Plot input signal
     */
    uart1.print(inputSignal);   uart1.print("\t");
    /* 
     * Plot output of first LPF. The amplitude of frequencies above BANDWIDTH_HZ
     * begin to roll off at 20dB per decade or 6 dB per octave
     */
    //uart1.print(lpf.NextValue(inputSignal));
    //uart1.print(" ");
    /* 
     * Plot output of the three cascade LPF.
     */
    uart1.println(lpf3.NextValue(inputSignal));
}
void PlotRisetimeResponse ( void ) {

    static unsigned long timer = millis();
    static float lpfInput = 5.0;
    unsigned long PW_MSEC = 1000.0 / (SIGNAL_FREQUENCY_HZ * 2.0);

    /* Allow the input to pulse */
    if ( (millis() - timer) >= PW_MSEC ) {
       // digitalWrite(PULSE_OUT,!digitalRead(PULSE_OUT));
        lpfInput = 5.0 * (float)inputValue / 1024.0;
        timer = millis();
    }
            
    /* 
     *  Plot input signal
     */
    //uart1.print(lpfInput);
    //uart1.print(" "); 
    /*
     * Plot impulse response output of LPF (it's risetime / falltime).
     * This is equal to ~ 0.35 / BANDWIDTH_HZ.
     */
   // uart1.print(lpf.NextValue(lpfInput));
   // uart1.print(" ");
    /*
     * Plot impulse response output of three cascade LPF (it's risetime / falltime).
     */        
    uart1.println(lpf3.NextValue(lpfInput));
}
Timer timer2(TIM2);
float filteredData;
void test()
{
 //   inputValue = testAmplitude + testAmplitude*sin( 2*PI * time()  );

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
    uart1.printf("\r\nuart1 115200 ok!\r\n");
    PB1.mode(AIN);
    timer2.begin(10);
    timer2.attach(test);
    timer2.interrupt(ENABLE);
    timer2.start();
    testOnePoleFilters();
}


int main(void)
{
    static uint64_t last_time = millis();
    static uint64_t last_time1 = millis();
    setup();

    while(1)
    {
//        if(millis() - last_time > 1)
//        {
//            last_time = millis();

//        }
//        if(millis() - last_time1 > 20)
//        {
//            last_time1 = millis();
//        }                    
        PlotSineWaveWithNoiseResponse();

        //delay_ms(100);
    }
}




