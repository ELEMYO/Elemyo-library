//  2018-04-18 by ELEMYO (https://github.com/ELEMYO/Arduino)

#include "ELEMYO.h"
#include "Arduino.h"
#include "SPI.h"

//     INITIATION 

ELEMYO::ELEMYO(byte pin) {
	_cs = pin;                              // choose the chip select pin
	pinMode(_cs,OUTPUT);                    // set the pin as output
    	digitalWrite(_cs,HIGH);                 // set the pin to default HIGH state
    	SPI.begin();                            // initiate SPI
    }

//  PUBLIC  METHODS


void ELEMYO::gain(byte value) {
	digitalWrite(_cs,LOW);              // select CHIP
	SPI.transfer(GAIN);                 // send GAIN command
	SPI.transfer(value);                // transfer data
	digitalWrite(_cs,HIGH);             // end transmission
	}



int ELEMYO::BandStop (int sensorValue, int f, float BW)
{  
 	short DT = micros()-T;			// time period from previous function call

	T = micros();				// time of function call
   
	//---Calculation of filter coefficients------------------------------------------
	float r = 1 - 0.00000314*BW*DT;   
	float b1 = -2*cos(0.000006283*f*DT);
	float a1 = r*b1; 
	float K = (1+a1+r*r)/(2+b1);
	//--------------------------------------------------------------------------------
	
	
	Y[2]=K*(sensorValue+b1*X[1]+X[0])-a1*Y[1]-r*r*Y[0];	//The new signal value after filtering

	//---save these values: x(n-1), x(n-2), y(n-1), y(n-2)---------------------------
	Y[0]=Y[1];
	Y[1]=Y[2];
	X[0]=X[1];
	X[1]=sensorValue;
	//--------------------------------------------------------------------------------
  
	return (int) Y[2];
}
