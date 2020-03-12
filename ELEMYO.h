//  2018-04-18 by ELEMYO (https://github.com/ELEMYO/Arduino)


#ifndef ELEMYO_h

	#define ELEMYO_h
	
	#include "Arduino.h"          // include arduino Core

	#define GAIN    B01000000     // write gain register

	#define x1     B000           // x1  gain
	#define x2     B001           // x2  gain
	#define x4     B010           // x4  gain
	#define x5     B011           // x5  gain
	#define x8     B100           // x8  gain
	#define x10    B101           // x10 gain
	#define x16    B110           // x16 gain
	#define x32    B111           // x32 gain

	class ELEMYO {
		public:
			ELEMYO(byte CS);		// initiate object
			void gain(byte value);		// change gain
            		float X[2] = {0, 0};		// array for BandStop filter with x(n-1) and x(n-2) values
            		float Y[3] = {0, 0, 0};		// array for BandStop filter with y(n), y(n-1) and y(n-2) values
	    		long T = 0;			// time in microseconds for BandStop filter
			
			// BandStop filter, sensorValue - signal value, f - notch frequency in Hz, BW - BandStop filter window in Hz
			int BandStop (int sensorValue, int f, float BW);	
                        
		private:
            		byte _cs;			// chip select pin
			int sensorInPin;		// chip select pin
	};

	
#endif

