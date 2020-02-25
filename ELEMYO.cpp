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
