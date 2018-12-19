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
#include "bsp_ebox.h"

#include "./FFT/src/arduinoFFT.h"

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
/*
These values can be changed in order to evaluate the functions
*/

const uint16_t samples = 64;
const double sampling = 40;
const uint8_t amplitude = 4;
uint8_t exponent;
const double startFrequency = 2;
const double stopFrequency = 16.4;
const double step_size = 0.1;

/*
These are the input and output vectors
Input vectors receive computed results from FFT
*/
double vReal[samples];
double vImag[samples];

unsigned long time;

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03


void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType);

void setup()
{
    ebox_init();
    UART.begin(115200);
    exponent = FFT.Exponent(samples);

}
uint32_t time1, time2;

int main(void)
{
    setup();
    while(1)
    {

        Serial.println("Frequency\tDetected\ttakes (ms)");
        Serial.println("=======================================\n");
        for(double frequency = startFrequency; frequency<=stopFrequency; frequency+=step_size)
        {
            /* Build raw data */
            double cycles = (((samples-1) * frequency) / sampling);
            for (uint16_t i = 0; i < samples; i++)
            {
                vReal[i] = int8_t((amplitude * (sin((i * (twoPi * cycles)) / samples))) / 2.0);
                vImag[i] = 0; //Reset the imaginary values vector for each new frequency
            }
            /*Serial.println("Data:");
            PrintVector(vReal, samples, SCL_TIME);*/
            time=millis();
            FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);	/* Weigh data */
            /*Serial.println("Weighed data:");
            PrintVector(vReal, samples, SCL_TIME);*/
            FFT.Compute(vReal, vImag, samples, exponent, FFT_FORWARD); /* Compute FFT */
            /*Serial.println("Computed Real values:");
            PrintVector(vReal, samples, SCL_INDEX);
            Serial.println("Computed Imaginary values:");
            PrintVector(vImag, samples, SCL_INDEX);*/
            FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
            /*Serial.println("Computed magnitudes:");
            PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);*/
            double x = FFT.MajorPeak(vReal, samples, sampling);
            Serial.print(frequency);
            Serial.print(": \t\t");
            Serial.print(x, 4);
            Serial.print("\t\t");
            Serial.print((int)(millis()-time));
            Serial.println(" ms");
            // delay(2000); /* Repeat after delay */
        }
        while(1); /* Run Once */
  }
}

