/*
 * lpf.cpp
 * 
 * Copyright 2016 Bill Williams <wlwilliams1952@gmail.com, github/BillWilliams1952>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General License for more details.
 * 
 * You should have received a copy of the GNU General License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include "Lpf.h"
#include "math.h"
/* Return RC low-pass filter output samples, given input samples,
 * time interval dt, and time constant RC.
 * 
 * α = dt / (RC + dt) where RC = 1 / (2 * π * BW)
 * y[0] := x[0]
 * for i from 1 to n
 *    y[i] = α * x[i] + (1-α) * y[i-1]
 * return y
 *
 * Refactoring
 *    for i from 1 to n
 *       y[i] = y[i-1] + α * (x[i] - y[i-1])
 *
 * OR: 	NextValue = LastValue + alpha * (CurrentValue - LastValue)
 * 		LastValue = NextValue
 *

 * The new output will approach the input in ~ 5 RCTime constants.
 * For example: With a specified bandwidth of 10 Hz, and a step
 * input applied, the output will settle to the step input value in
 * ~0.08 seconds.
 * 
 * 10 Hz is the 3dB point of the filter, where input changes at a
 * rate greater than this frequency begins to roll off at 20 dB per
 * decade (10 Hz, 100Hz, 1000Hz,...) or 6dB per octave (10Hz, 20Hz, 40Hz,
 * 80Hz...). 
 *
 * initialValue defaults to 0 if not provided.
 * 
 * cascades is the number of cascaded LPFs, ranging from 1 (default) to
 * MAX_CASCADES. Each LPF cascades its output to the input of the next LPF
 * in the cascade. The effect is the same as one LPF buffered by an OP-AMP
 * feeding another LPF .... etc.
 *
 */
 
LPF :: LPF ( double bandWidthInHzOrAlpha, bool isInBandwidth, uint8_t cascades ) {
	if ( isInBandwidth ) {
		alpha = 1.0;
		RCTime = bandWidthInHzOrAlpha <= 0 ?
					0.0 : 1.0 / (2*PI * bandWidthInHzOrAlpha);
	} else {
		alpha = constrain(bandWidthInHzOrAlpha,0.0,1.0); 
		RCTime = 0.0;
	}

	numCascades = constrain(cascades,1,MAX_CASCADES);
	/*
	 * TODO: In the unlikely event of a malloc fail..... what then?
	 * Could just say lastValue = &SomeFloatValue;
	 */
	lastValue = (double *) malloc(cascades * sizeof(double));
	Reset(0.0);
}

/*
 * Reset filter to initialValue (default is 0 if not provided)
 */
void LPF :: Reset ( double initialValue ) {
	for ( uint8_t i = 0; i < numCascades; i++ )
		lastValue[i] = initialValue;
	lastTimeInUsec = micros();
}

/*
 * Return last value from the filter.
 */
double LPF :: GetLastValue ( void ) {
	return lastValue[numCascades-1];
}

/*
 * Get next value from the filter based on the input (current) value
 * If this is a filter that uses bandwidth (i.e. RCTime != 0.0), then we
 * need to know the sample time from the last reading in order to
 * calculate the new alpha for the filter.
 * If this filter is just based on a fixed alpha, then just calculate a
 * new output, the sample time is irrelavent.
 */
double LPF :: NextValue ( double currentValue ) {
	double val;
	float sampleTimeInSec;
	
	if ( RCTime != 0.0 ) {
		sampleTimeInSec = ((float)micros() - (float)lastTimeInUsec) * 1.0e-6;
		if ( sampleTimeInSec < 0.0 ) {		// counter overrun
			lastTimeInUsec = micros();
			return lastValue[numCascades - 1];
		}
		alpha = sampleTimeInSec / (RCTime + sampleTimeInSec);
	}

	for ( uint8_t i = 0; i < numCascades; i++ ) {
		if ( i == 0 )	val = currentValue;
		else            val = lastValue[i-1];
		lastValue[i] = lastValue[i] + alpha * (val - lastValue[i]);
	}
	lastTimeInUsec = micros();
	return lastValue[numCascades - 1];
}

/*
 * Return the gain of the filter at the requested frequency
 */
double LPF :: GetFilterGainInDB ( double frequencyInHz ) {
	 // not frequency dependent or bad frequency - ignore
	if ( RCTime == 0.0 || frequencyInHz <= 0.0 ) return 0;

	double ratio = frequencyInHz * TWO_PI * RCTime;
	double gain = 1.0 / sqrt((ratio*ratio + 1));
	//for ( int i=1; i < numCascades; i++) {
	//	gain *= gain;
	//}
	return 20.0 * log10(pow(gain,numCascades));
}

/*
 * Return the required signal frequency that develops the requested output gain
 */
double LPF :: GetFrequencyForGain ( double gainInDB ) {
	// not frequency dependent or positive gain - ignore
	if ( RCTime == 0.0 || gainInDB > 0.0 ) return 0; 

	float val = 1.0 / exp( log( pow(10.0,gainInDB/20.0) ) / numCascades );
	return (1.0 / (TWO_PI * RCTime)) * sqrt((val * val - 1.0));
}


