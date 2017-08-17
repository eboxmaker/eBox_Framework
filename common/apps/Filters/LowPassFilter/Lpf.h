/*
 * Lpf.h
 * 
 * Copyright 2016 Bill Williams <wlwilliams1952@gmail.com, github/BillWilliams1952>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#ifndef __LPF_H__

#define __LPF_H__

#include <ebox.h>

#define MAX_CASCADES		5		// arbitrary
#define IS_BANDWIDTH_HZ		true
#define	IS_ALPHA			false

class LPF {

	public:

		// Specify a LPF based off bandwidth, or as fixed alpha (α).
		LPF ( double bandWidthInHzOrAlpha, bool isInBandwidth, uint8_t cascades = 1 );

		// Reset filter output to some initialValue
		void Reset ( double initialValue =  0.0 );

		// Return last value from the filter.
		double GetLastValue ( void );

		// Get next value from the filter based on the input (current) value
		double NextValue ( double currentValue );

		// Return the gain of the filter at the requested frequency
		double GetFilterGainInDB ( double frequencyInHz );

		// Return the required signal frequency that develops the requested output gain
		double GetFrequencyForGain ( double gainInDB );
	
	private:
		
		uint8_t numCascades;
		double RCTime, alpha, * lastValue;
		unsigned long lastTimeInUsec;
};

#endif
