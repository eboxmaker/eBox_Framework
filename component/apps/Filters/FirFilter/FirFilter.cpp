/*
  FirFilter.cpp
  Finite Impulse Response Filter library for the eBox 32bit Cortex-M processors
  Copyright (c) 2015 Phillip Schmidt.  All right reserved.

  All operations during filtering are done with integer math to increase speed.
  
  http://www.labbookpages.co.uk/audio/firWindowing.html
  
	!!! All input data must be type INT.  !!!
	
	Processing Time on an 72Mhz STM32
	47 Window = 23.7us
	57 Window = 28.6us
	74 Window = 33us
	94 Window = 41.7us
    113 Window = 49.9us
    225 Window =  98.5us
 
*/

#include "FirFilter.h"
	

int FirFilter::in(int value) // data input function
{  
	
	DataList[OldestDataPoint] = value; // replace oldest data in list
	
	// multiply sample by coefficient
	result.L = DataList[OldestDataPoint]*CoefList[0]; // accumulate data in a Long to eliminate rounding errors
	
	for(int i = 1; i < filterWindowSize; i++) // iterate through the rest of the data (stopping one short of start point)
	{
		OldestDataPoint++;	// increment and wrap pointer
		if(OldestDataPoint == filterWindowSize)  OldestDataPoint = 0;
		
		// multiply sample by coefficient 
		result.L += DataList[OldestDataPoint]*CoefList[i];
	}
	
	result.L = result.L << 1; // bitshift to make up for non 16bit coefficient offset
	return result.I[1]; // the H16 part of the Long is the result
}


float FirFilter::in(float value) // data input function
{  
	//需要将DataList，CoefList除以32767变换回小数
    float resultx;
	DataList[OldestDataPoint] = value; // replace oldest data in list
	
	// multiply sample by coefficient
	resultx = DataList[OldestDataPoint]*CoefList[0]; // accumulate data in a Long to eliminate rounding errors
	
	for(int i = 1; i < filterWindowSize; i++) // iterate through the rest of the data (stopping one short of start point)
	{
		OldestDataPoint++;	// increment and wrap pointer
		if(OldestDataPoint == filterWindowSize)  OldestDataPoint = 0;
		
		// multiply sample by coefficient 
		resultx += DataList[OldestDataPoint]* CoefList[i];
	}
	
	
	return resultx; // the H16 part of the Long is the result
}


int FirFilter::out() // return the value of the most recent result 
{
	return result.I[1];;   
}




// multiplication wrapper function
int32_t inline FirFilter::mul(int16_t a, int16_t b) {
	int32_t r;
    r = a*b;
	return r;
}

// Create sinc function for filter with 1 transition - Low and High pass filters
float *FirFilter::create1TransSinc(int windowLength, float transFreq, float sampFreq, enum filterType type)
{
	int n;

	// Allocate memory for the window
	float *window = (float *) ebox_malloc(windowLength * sizeof(float));
	if (window == NULL) {
		uart1.printf( "create1TransSinc: Could not allocate memory for window\n");
		return NULL;
	}

	if (type != LOW_PASS && type != HIGH_PASS) {
		uart1.printf( "create1TransSinc: Bad filter type, should be either LOW_PASS of HIGH_PASS\n");
		return NULL;
	}

	// Calculate the normalised transistion frequency. As transFreq should be
	// less than or equal to sampFreq / 2, ft should be less than 0.5
	float ft = transFreq / sampFreq;

	float m_2 = 0.5 * (windowLength-1);
	int halfLength = windowLength / 2;

	// Set centre tap, if present
	// This avoids a divide by zero
	if (2*halfLength != windowLength) {
		float val = 2.0 * ft;

		// If we want a high pass filter, subtract sinc function from a dirac pulse
		if (type == HIGH_PASS) val = 1.0 - val;

		window[halfLength] = val;
	}
	else if (type == HIGH_PASS) {
		uart1.printf("create1TransSinc: For high pass filter, window length must be odd\n");
		return NULL;
	}

	// This has the effect of inverting all weight values
	if (type == HIGH_PASS) ft = -ft;

	// Calculate taps
	// Due to symmetry, only need to calculate half the window
	for (n=0 ; n<halfLength ; n++) {
		float val = sin(2.0 * PI * ft * (n-m_2)) / (PI * (n-m_2));

		window[n] = val;
		window[windowLength-n-1] = val;
	}

	return window;
}

// Create two sinc functions for filter with 2 transitions - Band pass and band stop filters
float *FirFilter::create2TransSinc(int windowLength, float trans1Freq, float trans2Freq, float sampFreq, enum filterType type)
{
	int n;

	// Allocate memory for the window
	float *window = (float *) ebox_malloc(windowLength * sizeof(float));
	if (window == NULL) {
		uart1.printf("create2TransSinc: Could not allocate memory for window\n");
		return NULL;
	}

	if (type != BAND_PASS && type != BAND_STOP) {
		uart1.printf("create2TransSinc: Bad filter type, should be either BAND_PASS or BAND_STOP\n");
		return NULL;
	}

	// Calculate the normalised transistion frequencies.
	float ft1 = trans1Freq / sampFreq;
	float ft2 = trans2Freq / sampFreq;

	float m_2 = 0.5 * (windowLength-1);
	int halfLength = windowLength / 2;

	// Set centre tap, if present
	// This avoids a divide by zero
	if (2*halfLength != windowLength) {
		float val = 2.0 * (ft2 - ft1);

		// If we want a band stop filter, subtract sinc functions from a dirac pulse
		if (type == BAND_STOP) val = 1.0 - val;

		window[halfLength] = val;
	}
	else {
		uart1.printf("create1TransSinc: For band pass and band stop filters, window length must be odd\n");
		return NULL;
	}

	// Swap transition points if Band Stop
	if (type == BAND_STOP) {
		float tmp = ft1;
		ft1 = ft2; ft2 = tmp;
	}

	// Calculate taps
	// Due to symmetry, only need to calculate half the window
	for (n=0 ; n<halfLength ; n++) {
		float val1 = sin(2.0 * PI * ft1 * (n-m_2)) / (PI * (n-m_2));
		float val2 = sin(2.0 * PI * ft2 * (n-m_2)) / (PI * (n-m_2));

		window[n] = val2 - val1;
		window[windowLength-n-1] = val2 - val1;
	}

	return window;
}
// Create a set of window weights
// in - If not null, each value will be multiplied with the window weight
// out - The output weight values, if NULL and new array will be allocated
// windowLength - the number of weights
// windowType - The window type
float *FirFilter::createWindow(float *in, float *out, int windowLength, enum windowType type)
{
	// If output buffer has not been allocated, allocate memory now
	if (out == NULL) {
		out = (float *) ebox_malloc(windowLength * sizeof(float));
		if (out == NULL) {
			uart1.printf("Could not allocate memory for window\n");
			return NULL;
		}
	}

	int n;
	int m = windowLength - 1;
	int halfLength = windowLength / 2;

	// Calculate taps
	// Due to symmetry, only need to calculate half the window
	switch (type)
	{
		case RECTANGULAR:
			for (n=0 ; n<windowLength ; n++) {
				out[n] = 1.0;
			}
			break;

		case BARTLETT:
			for (n=0 ; n<=halfLength ; n++) {
				float tmp = (float) n - (float)m / 2;
				float val = 1.0 - (2.0 * fabs(tmp))/m;
				out[n] = val;
				out[windowLength-n-1] = val;
			}

			break;

		case HANNING:
			for (n=0 ; n<=halfLength ; n++) {
				float val = 0.5 - 0.5 * cos(2.0 * PI * n / m);
				out[n] = val;
				out[windowLength-n-1] = val;
			}

			break;

		case HAMMING:
			for (n=0 ; n<=halfLength ; n++) {
				float val = 0.54 - 0.46 * cos(2.0 * PI * n / m);
				out[n] = val;
				out[windowLength-n-1] = val;
			}
			break;

		case BLACKMAN:
			for (n=0 ; n<=halfLength ; n++) {
				float val = 0.42 - 0.5 * cos(2.0 * PI * n / m) + 0.08 * cos(4.0 * PI * n / m);
				out[n] = val;
				out[windowLength-n-1] = val;
			}
			break;
	}

	// If input has been given, multiply with out
	if (in != NULL) {
		for (n=0 ; n<windowLength ; n++) {
			out[n] *= in[n];
		}
	}

	return out;
}

// ripple is given in 0.001-x,描述文波
// Transition Width (transWidth) is given in Hz
// Sampling Frequency (sampFreq) is given in Hz
// Window Length (windowLength) will be set
void FirFilter::calculateKaiserParams(float ripple, float transWidth, float sampFreq, int *windowLength, float *beta)
{
	// Calculate delta w
	double dw = 2 * PI * transWidth / sampFreq;

	// Calculate ripple dB
	double a = -20.0 * log10(ripple);

	// Calculate filter order
	int m;
	if (a>21) m = ceil((a-7.95) / (2.285*dw));
	else m = ceil(5.79/dw);

	*windowLength = m + 1;

	if (a<=21) *beta = 0.0;
	else if (a<=50) *beta = 0.5842 * pow(a-21, 0.4) + 0.07886 * (a-21);
	else *beta = 0.1102 * (a-8.7);
}

float *FirFilter::createKaiserWindow(float *in, float *out, int windowLength, float beta)
{
	float m_2 = (float)(windowLength-1) / 2.0;
	float denom = modZeroBessel(beta);					// Denominator of Kaiser function

	// If output buffer has not been allocated, allocate memory now
	if (out == NULL) {
		out = (float *) ebox_malloc(windowLength * sizeof(float));
		if (out == NULL) {
			uart1.printf( "Could not allocate memory for window\n");
			return NULL;
		}
	}

	int n;
	for (n=0 ; n<windowLength ; n++)
	{
		float val = ((n) - m_2) / m_2;
		val = 1 - (val * val);
		out[n] = modZeroBessel(beta * sqrt(val)) / denom;
	}

	// If input has been given, multiply with out
	if (in != NULL) {
		for (n=0 ; n<windowLength ; n++) {
			out[n] *= in[n];
		}
	}

	return out;
}
float FirFilter::modZeroBessel(float x)
{
	int i;

	float x_2 = x/2;
	float num = 1;
	float fact = 1;
	float result = 1;

	for (i=1 ; i<20 ; i++) {
		num *= x_2 * x_2;
		fact *= i;
		result += num / (fact * fact);
//		printf("%f %f %f\n", num, fact, result);
	}

	return result;
}