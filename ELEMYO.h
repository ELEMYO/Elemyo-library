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
			ELEMYO(byte CS);         // initiate object
			void gain(byte value);     // change gain
        
		private:
            byte _cs;                  // chip select pin
	};
	
#endif
