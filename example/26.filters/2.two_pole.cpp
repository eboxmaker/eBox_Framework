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
#include <Filters.h>

float lowFrequency = 2;                     // test signal frequency (Hz)
float highFrequency = 80;                     // test signal frequency (Hz)
float testAmplitude = 100;                   // test signal amplitude
float testOffset = 100;

float windowLength = 20.0/lowFrequency;     // how long to average the signal, for statistist

float testSignalSigma = testAmplitude / sqrt(2.0);         // the RMS amplitude of the test signal
float testSignal3dbSigma = testSignalSigma / sqrt(2.0);    // the RMS amplitude of the test signal, down -3db

float printPeriod = 5.0;


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
  while( true ) {
    // update all real time classes
    float inputValue = testAmplitude + testAmplitude*sin( 2*PI * 10 * time() ) + testAmplitude*sin( 2*PI * 100 * time() );

    // update the test value statistics
    inputStats.input( inputValue);
    
    // update the one pole lowpass filter, and statistics
    filterOneLowpass.input( inputValue );
    filterOneLowpassStats.input( filterOneLowpass.output() );
    
    // update the one pole highpass filter, and statistics
    filterOneHighpass.input( inputValue );
    filterOneHighpassStats.input( filterOneHighpass.output() );
    uart1.printf("%0.2f\t%0.2f\t%0.2f\r\n",inputValue,filterOneLowpass.output(),filterOneHighpass.output());

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
  }
}
void testTwoPoleFilters() {
  float factor = sqrt(10.0);  // sqrt 10 will make the attenuation 10x
  
  // filters are test with a sine wave input, keep track of those values here for a sanity check
  RunningStatistics inputStats;                 // create statistics to look at the raw test signal
  inputStats.setWindowSecs( windowLength );

  // standard Lowpass, set to the corner frequency
  FilterTwoPole filterTwoLowpass;                                       // create a two pole Lowpass filter
  filterTwoLowpass.setAsFilter( LOWPASS_BUTTERWORTH, lowFrequency );

  RunningStatistics filterTwoLowpassStats;
  filterTwoLowpassStats.setWindowSecs( windowLength );

  // Lowpass, set above corner frequency
  FilterTwoPole filterTwoLowpassAbove;                                       // create a two pole Lowpass filter
  filterTwoLowpassAbove.setAsFilter( LOWPASS_BUTTERWORTH, lowFrequency*factor );

  RunningStatistics filterTwoLowpassAboveStats;
  filterTwoLowpassAboveStats.setWindowSecs( windowLength );

  // Lowpass, set below corner frequency
  FilterTwoPole filterTwoLowpassBelow;                                       // create a two pole Lowpass filter
  filterTwoLowpassBelow.setAsFilter( LOWPASS_BUTTERWORTH, lowFrequency/factor );

  RunningStatistics filterTwoLowpassBelowStats;
  filterTwoLowpassBelowStats.setWindowSecs( windowLength );

  float startTime = time();
  float nextPrintTime = time();
  
  while( true ) {
    // update all real time classes
    float inputValue = testAmplitude + testAmplitude*sin( TWO_PI * lowFrequency * time()) + testAmplitude*sin( TWO_PI * highFrequency * time());
        
    // update the test value statistics
    inputStats.input( inputValue);
        
    // update the two pole Lowpass filter, and statistics
    filterTwoLowpass.input( inputValue );
    filterTwoLowpassStats.input( filterTwoLowpass.output() );
    
    // update the two pole Lowpass filter, and statistics
    filterTwoLowpassAbove.input( inputValue );
    filterTwoLowpassAboveStats.input( filterTwoLowpassAbove.output() );

    // update the two pole Lowpass filter, and statistics
    filterTwoLowpassBelow.input( inputValue );
    filterTwoLowpassBelowStats.input( filterTwoLowpassBelow.output() );
        uart1.printf("%0.2f\t%0.2f\t%0.2f\t%0.2f\r\n",inputValue,filterTwoLowpass.output(),filterTwoLowpassAbove.output(),filterTwoLowpassBelow.output());

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
//      // output values associated with the Lowpass value
//      uart1.print( "\tLP2: " ); uart1.print( filterTwoLowpassStats.mean() ); uart1.print( " +/- " ); uart1.print( filterTwoLowpassStats.sigma() );
//      uart1.print( " (" ); uart1.print( "0.0" ); uart1.print( " +/- " ); uart1.print( testSignal3dbSigma ); uart1.print( ")" );
//      
//      // output values associated with the Lowpass value, set above corner frequency (should be almost full)
//      uart1.print( "\tLP2A: " ); uart1.print( filterTwoLowpassAboveStats.mean() ); uart1.print( " +/- " ); uart1.print( filterTwoLowpassAboveStats.sigma() );

//      // output values associated with the Lowpass value, set above corner frequency (should be lower amplitude)
//      uart1.print( "\tLP2B: " ); uart1.print( filterTwoLowpassBelowStats.mean() ); uart1.print( " +/- " ); uart1.print( filterTwoLowpassBelowStats.sigma() );
//    }
  }
}
void setup()
{
    double Answer[5], SquarePoor[4];
    ebox_init();
    uart1.begin(115200);
    uart1.printf("\r\nuart1 115200 ok!\r\n");
    PB1.mode(AIN);
    testTwoPoleFilters();
}


int main(void)
{
    static uint64_t last_time = millis();
    static uint64_t last_time1 = millis();
    setup();

    while(1)
    {
        if(millis() - last_time > 1)
        {
            last_time = millis();

        }
        if(millis() - last_time1 > 20)
        {
            last_time1 = millis();
        }                    

        //delay_ms(100);
    }
}




