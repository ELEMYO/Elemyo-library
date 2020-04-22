//  Reads an analog input on pin A0, prints the result to the Serial Monitor.
//  Graphical representation is available using Matlab Terminal.
//  2018-04-18 by ELEMYO (https://github.com/ELEMYO/Elemyo-library)
//
// Changelog:
//     2020-04-22 - initial release

/* ============================================
ELEMYO library code is placed under the MIT license
Copyright (c) 2018 ELEMYO

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================


Wiring the ELEMYO sensor to an Arduino

ELEMYO -->  Arduino
  +        5V
  -        GND
  S        analog pin A0 (or other analog input)
  СS       pin 10 (SS pin)
  MOSI     pin 11 (UNO, Nano, Mini), pin 51 (Mega), pin SI (Leonardo), pin 16 (Pro micro)
  SCK      pin 13 (UNO, Nano, Mini), pin 52 (Mega), pin SCK (Leonardo), pin 15 (Pro micro)

*/

#include <ELEMYO.h>

#define   CSpin         10
#define   sensorInPin   A0     // analog input pin that the sensor is attached to

int sensorValue = 0;           // value read from the sensor
int bandStopValue = 0;         // result of filtration by BandStop filter
int bandPassValue = 0;         // result of filtration by BandPass filter
int lowPassValue = 0;          // result of filtration by LowPass filter

ELEMYO MyoSensor(CSpin);

void setup() {
  // initialize serial communications at 115200 bps:
  Serial.begin(115200);
  // initial value of gain: x1; x2; x4; x5; x8; x10; x16; x32
  MyoSensor.gain(x1);
  // initialisation of sensorInPin
  pinMode(sensorInPin, INPUT);
}

void loop() {
  
  // read the analog input value:
  sensorValue = analogRead(sensorInPin);
  
  // notch 50 Hz filter with band window 4 Hz.
  bandStopValue = MyoSensor.BandStop(sensorValue, 50, 4);

  // Chebyshev 6th order 1dB ripple low pass filter for 80 Hz. 
  // Sampling (loop) frequency must be more then 2* cutoff frequency.
  // Cutoff frequency should be more than 30 Hz.
  lowPassValue = MyoSensor.LowPass(sensorValue, 80, 2);

  // Chebyshev 4th order 1dB ripple bandpass filter for 30 - 80 Hz. 
  // Sampling (loop) frequency must be more then 2* upper cutoff frequency.
  // Lower cutoff frequency should be more than 30 Hz.
  // Difference between a lower and an upper cutoff frequencies should be more than 10 Hz
  bandPassValue = MyoSensor.BandPass(sensorValue, 30, 80, 2);

  //print the results to the Serial Monitor:
  Serial.print(sensorValue);
  Serial.print(" ");
  Serial.println(bandStopValue);
  //Serial.println(lowPassValue);
  //Serial.println(bandPassValue);



  // wait before the next loop
  // delay more than 4ms can broke BandPass and LowPass filters with 80 Hz cutoff frequency.
  delay(0);
}
