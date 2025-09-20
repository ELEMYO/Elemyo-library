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

	class BANDSTOP {
		public:
			float X[2] = {0, 0};				// array for BandStop filter with x(n-1) and x(n-2) values
            float Y[3] = {0, 0, 0};				// array for BandStop filter with y(n), y(n-1) and y(n-2) values
	    	long T = 0;							// time in microseconds for BandStop filter
			float f = 0;						// notch frequency in Hz
			float BW = 0;						// notch window in Hz

			void INITIAL();						// initilisation
	};

	class LOWPASS {
		public:
            float X[6] = {0, 0, 0, 0, 0, 0};		// array for LowPass filter with x(n-1) ... x(n-6) values
            float Y[7] = {0, 0, 0, 0, 0, 0, 0};		// array for LowPass filter with y(n) ... y(n-6) values
			long T = 0;								// time in microseconds for LowPass filter
			float f = 0;							// cutoff frequency in Hz
			
			void INITIAL();							// initilisation
	};

    class HIGHPASS {
		public:
            float X[1] = {0};			// array for LowPass filter with x(0) value
            float Y[2] = {0, 0};		// array for LowPass filter with y(0) and y(1) values
			long T = 0;					// time in microseconds for LowPass filter
			float f = 0;				// cutoff frequency in Hz

			void INITIAL();				// initilisation
	};

	class BANDPASS {
		public:
            float X[8] = {0, 0, 0, 0, 0, 0, 0, 0};		// array for BandPass filter with x(n-1) ... x(n-8) values
            float Y[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};	// array for BandPass filter with y(n) ... y(n-8) values
			long T = 0;									// time in microseconds for BandPass filter
			float fl = 0;								// lower cutoff frequency in Hz
			float fh = 0;								// upper cutoff frequency in Hz

			void INITIAL();								// initilisation
	};

	class ELEMYO {
		public:
			ELEMYO(){};							// initiate empty object
			ELEMYO(byte CS) {_cs = CS;};		// initiate object
			void begin();
			void gain(byte value);				// change gain
		
			// BandStop filter, "sensorValue" - signal value, "f" - notch frequency in Hz, "BW" - notch filter window in Hz
			int BandStop (int sensorValue, float f, float BW);	

			// 6th order lowpass filter, "sensorValue" - signal value, "f" - cutoff frequency in Hz (≥30 Hz)
			// "type" - filter type: 0 - Butterworth, 1 - Chebyshev with 0.5 dB ripple, 2 - Chebyshev with 1 dB ripple
			int LowPass (int sensorValue, float f, int type);
			
            // 1th order Butterworth highpass filter, "sensorValue" - signal value, "f" - cutoff frequency in Hz (≥1 Hz)
            int HighPass (int sensorValue, float f);

			// 4th order bandpass filter, "sensorValue" - signal value, "fl" - lower cutoff frequency in Hz (≥30 Hz), "fh" - upper cutoff frequency in Hz (fh≥(fl+10)),
			// "type" - filter type: 0 - Butterworth, 1 - Chebyshev with 0.5 dB ripple, 2 - Chebyshev with 1 dB ripple
			int BandPass (int sensorValue, float fl, float fh, int type);
		
		    // Exponential moving average, "sensorValue" - signal value, "signalReference" - reference of signal, "alpha" - smoothing constant
			int movingAverage (int sensorValue, float alpha);
                        
		private:
            byte _cs;			// chip select pin
			int sensorInPin;	// chip select pin

			int NumBS = 0;		// number of BandStop filters with different notch frequency
			BANDSTOP BS[2];		// array of BandStop filters with different notch frequency

			int NumLP = 0;		// number of LowPass filters with different cutoff frequency
			LOWPASS LP[1];		// array of LowPass filters with different cutoff frequency

			int NumHP  = 0;		// number of HIGHPASS filters with different cutoff frequency
			HIGHPASS HP[1];		// array of HIGHPASS filters with different cutoff frequency

			int NumBP = 0;		// number of BANDPASS filters with different lower or upper cutoff frequencys
			BANDPASS BP[1];		// array of BANDPASS filters with different lower or upper cutoff frequencys

			int LengthMA = 10;                                  // length of moving average massive
			float MA[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};      // array for moving average method
	};
	
#endif
