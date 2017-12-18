/*
  FirFilter.h 
  Copyright (c) 2013 Phillip Schmidt.  All right reserved.

 */

#ifndef FirFilter_h

	#define FirFilter_h

	#include "ebox.h"
    
enum filterType {LOW_PASS, HIGH_PASS, BAND_PASS, BAND_STOP};
enum windowType {RECTANGULAR, BARTLETT, HANNING, HAMMING, BLACKMAN};

	class FirFilter
	{
	
		public: 
			FirFilter(){};
			int in(int value);
			float in(float value);
			int out();
            float *create1TransSinc(int windowLength, float transFreq, float sampFreq, enum filterType type);
            float *create2TransSinc(int windowLength, float trans1Freq, float trans2Freq, float sampFreq, enum filterType type);
            float *createWindow(float *in, float *out, int windowLength, enum windowType type);
            void calculateKaiserParams(float ripple, float transWidth, float sampFreq, int *windowLength, float *beta);
            float *createKaiserWindow(float *in, float *out, int windowLength, float beta);
            float modZeroBessel(float x);
                
            void set(float *p,int windowLength)
            {
                filterWindowSize = windowLength;
                DataList = (int*) ebox_malloc (windowLength * sizeof(int));		// create array for data
                CoefList = (int*) ebox_malloc (windowLength * sizeof(int));		// create array for coefficients
                for(int i = 0; i < windowLength; i++)
                    CoefList[i] = 32767.0 *  (*p++);
                ebox_free(&p);
            }

			int * DataList;			// array for data	
			int * CoefList;			// array for coefficients	
		
		private:
			int32_t mul(int16_t a, int16_t b);
			
			uint8_t filterWindowSize;	// number of samples window	
			uint8_t OldestDataPoint;	// oldest data point location	
			
			
			union TypeConverter
			{
				int32_t L;
				int16_t  I[2];
			} result;

	};

#endif
