# Low-Pass-Filter

Simple low pass filter (LPF) based on sampling theory.  

This library allows the user to specify the filter bandwidth in hertz (Hz) of the LPF. The LPF will then calculate the response based on the sample time between calls.  Multiple LPFs may be cascaded together using the optional *cascades* parameter, increasing the rolloff at the output.

If sample time is not an issue, then the LPF can be created using a simple alpha value which ranges from 0.0 to 1.0. A value of 1.0 is no filtering (follow the input), 0.0 is max filtering (the output stays at the initial value).

##Synopsis

Return RC low-pass filter output samples, given input samples, time interval dt, and time constant RC. From sampling theory:

	RC = 1 / (2 * π * BW)
	α = dt / (RC + dt)
	y[0] := x[0]
	for i from 1 to n
		y[i] = α * x[i] + (1-α) * y[i-1]

Refactoring

	for i from 1 to n
		y[i] = y[i-1] + α * (x[i] - y[i-1])

Rewriting

	NextValue = LastValue + α * (CurrentValue - LastValue)
	LastValue = NextValue
	
For a single LPF, the new output will approach the input in ~ 5 RCTime constants. 

For example: With a specified bandwidth of 10 Hz, and a step input applied, the output will settle to the step input value in ~0.08 seconds. 10 Hz is the 3dB point of the filter, where input changes at a rate greater than this frequency begins to roll off at 20 dB per decade (10 Hz, 100Hz, 1000Hz,...) or 6dB per octave (10Hz, 20Hz, 40Hz, 80Hz...)
![alt tag](https://cloud.githubusercontent.com/assets/3778024/21202816/901c5764-c215-11e6-9895-a39fdd9bd3f0.png)
Thanks https://en.wikipedia.org/wiki/Roll-off

When you cascade the LPF, the rolloff basically doubles for each added stage.  Using the previous example, a two cascade LPF will be 6dB down at 10Hz, and 40 dB down at 100 Hz -- a three cascade LPF will be 9dB down at 10Hz, and 60 dB down at 100 Hz.

##API

	// Specify a LPF based off bandWidth, or as fixed alpha.
	LPF ( double bandWidthInHzOrAlpha, bool isInBandwidth, uint8_t cascades = 1 );

	// Reset filter output to initialValue
	void Reset ( float initialValue =  0.0 );

	// Return last value from the filter.
	float GetLastValue ( void );

	// Get next value from the filter based on the input (current) value
	float NextValue ( float currentValue );
	
	// Return the gain of the filter at the requested frequency using the formula
	
![alt tag](https://cloud.githubusercontent.com/assets/3778024/22393882/7bd96862-e4d6-11e6-8053-05b89cf9978e.png)

	double GetFilterGainInDB ( double frequencyInHz );

	// Return the required signal frequency that develops the requested output gain using the formula
	
![alt tag](https://cloud.githubusercontent.com/assets/3778024/22394159/ad50fb06-e4dd-11e6-9b22-96d9da1c1ea3.png)

	double GetFrequencyForGain ( double gainInDB );
  
## Tests

The example sketch uses the Serial Plotter functionality to display two examples. The first example plots the response of a single LPF and a three cascade LPF to a pulse input. This demonstrates the impulse response (risetime/falltime) of the LPF based on the *bandWidthInHzOrAlpha* parameter. Note that risetime or falltime is ~ 0.35 / *bandWidthInHzOrAlpha* for a single cascade LPF. 

![alt tag](https://cloud.githubusercontent.com/assets/3778024/22360065/2d42c230-e412-11e6-9cef-5dc5463204e3.png)

The second example plots the response of a single LPF and a three cascade LPF to an input containing multiple sine wave signals and 'noise'. The actual test suite now resets the three cascade LPF output to 1.0 every five seconds as a demonstration of the **Reset** function.

![alt tag](https://cloud.githubusercontent.com/assets/3778024/22360073/39756224-e412-11e6-8182-a1a94617328c.png)

## License

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses/.


