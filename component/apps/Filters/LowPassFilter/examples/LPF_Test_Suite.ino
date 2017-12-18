
/*
 * LPF_Test_Suite.ino
 * 2017 WLWilliams
 * 
 * This sketch demonstrates the use of the LPF library. Use the Serial Plotter under Tools to
 * see the effects of a single or casscaded LPF on the inputs.
 * 
 * This program is free software: you can redistribute it and/or modify it under 
 * the terms of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version. 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of
 * the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Library originally added December 2016 by Bill Williams
 *
 * This example code is in the public domain.
 * 
 * https://github.com/Billwilliams1952/Low-Pass-Filter
 */

#include <Lpf.h>

#define PULSE_OUT             10      // Your arduino pin
#define ANALOG_IN             A0      // Your arduino pin

#define BANDWIDTH_HZ          1.0     // 3-dB bandwidth of the filter
#define SAMPLE_TIME_SEC       20e-3   // How often are we upodating the loop?
                                      // The LPF tracks the sample time internally
#define SIGNAL_FREQUENCY_HZ   0.3     // Our test input signal frequency

#define DO_PLOTTING                   // If not defined, test other functions in LPF

/*
 * Create a Low Pass Filter - it's default cascade is 1 and initialization is 0.0.  To change it, 
 * call Reset with an initialValue.
 */
LPF lpf(BANDWIDTH_HZ,IS_BANDWIDTH_HZ);      

/*
 * Create a three cascade LPF
 */
LPF lpf3(BANDWIDTH_HZ,IS_BANDWIDTH_HZ,3);

/*
 * Create a LPF with a fixed alpha
 */
LPF lpfFixedAlpha(0.1,IS_ALPHA);

void setup() {
    Serial.begin(115200);

    while (!Serial) ;

    pinMode(PULSE_OUT,OUTPUT);
    digitalWrite(PULSE_OUT,HIGH);
    pinMode(ANALOG_IN,INPUT);

#ifndef DO_PLOTTING
    /*
     * Test some of the functions
     */

    /* Should return ~ -20 dB */
    Serial.println(lpf.GetFilterGainInDB(BANDWIDTH_HZ * 10),5);
    /* Should return ~ -9 dB */
    Serial.println(lpf3.GetFilterGainInDB(BANDWIDTH_HZ),5);

    /* Should return ~ BANDWIDTH_HZ * 10 */
    Serial.println(lpf.GetFrequencyForGain(-20.0),5);  
    /* Should return ~ BANDWIDTH_HZ */
    Serial.println(lpf3.GetFrequencyForGain(-9.0),5);

    while ( 1 );    // Nothing to see here.
#endif
}

void loop() {
    
    digitalWrite(13,millis()%1000 > 500);   // I'm alive blinker

    /*
     * Uncomment the test you wish to visualize
     */
     
    // PlotRisetimeResponse();

    PlotSineWaveWithNoiseResponse();

    delay(SAMPLE_TIME_SEC*1000);
}

/*
 * Plot filter response to ac signals using serial plotter
 */
void PlotSineWaveWithNoiseResponse ( void ) {

    static unsigned long timer = millis();
    static unsigned int dt = 0;
    float inputSignal, twoPiDt = 2.0 * PI * dt * SAMPLE_TIME_SEC;

    inputSignal = 5.0 * sin(SIGNAL_FREQUENCY_HZ * twoPiDt) + 
                  /* now add some higher order signals to filter out */
                  2.0 * sin(BANDWIDTH_HZ * 10 * twoPiDt) +
                  1.0 * sin(BANDWIDTH_HZ * 20 * twoPiDt) +
                  /* some 'noise' */ (float)random(-100, 100) / 100.0;
    dt++;

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
    Serial.print(inputSignal);   Serial.print(" ");
    /* 
     * Plot output of first LPF. The amplitude of frequencies above BANDWIDTH_HZ
     * begin to roll off at 20dB per decade or 6 dB per octave
     */
    Serial.print(lpf.NextValue(inputSignal));
    Serial.print(" ");
    /* 
     * Plot output of the three cascade LPF.
     */
    Serial.println(lpf3.NextValue(inputSignal));
}

/*
 * Plot risetime / falltime using serial plotter
 */
void PlotRisetimeResponse ( void ) {

    static unsigned long timer = millis();
    static float lpfInput = 5.0;
    unsigned long PW_MSEC = 1000.0 / (SIGNAL_FREQUENCY_HZ * 2.0);

    /* Allow the input to pulse */
    if ( (millis() - timer) >= PW_MSEC ) {
        digitalWrite(PULSE_OUT,!digitalRead(PULSE_OUT));
        lpfInput = 5.0 * (float)analogRead(ANALOG_IN) / 1024.0;
        timer = millis();
    }
            
    /* 
     *  Plot input signal
     */
    Serial.print(lpfInput);
    Serial.print(" "); 
    /*
     * Plot impulse response output of LPF (it's risetime / falltime).
     * This is equal to ~ 0.35 / BANDWIDTH_HZ.
     */
    Serial.print(lpf.NextValue(lpfInput));
    Serial.print(" ");
    /*
     * Plot impulse response output of three cascade LPF (it's risetime / falltime).
     */        
    Serial.println(lpf3.NextValue(lpfInput));
}



