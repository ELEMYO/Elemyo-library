// Example of exponentially weighted moving average method (EWMA).
// Reads a signal from an analog input on pin A0, make a rectification of the signal and then realise triple exponential moving average.
// Prints the result to the Serial Monitor.
//  2018-04-18 by ELEMYO (https://github.com/ELEMYO/Elemyo-library)
//
// Changelog:
//     2020-02-24 - initial release

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
  СS       diginal pin 10  (SS pin)
  MOSI     pin 11 (UNO, Nano, Mini), pin 51 (Mega), pin SI (Leonardo), pin 16 (Pro micro)
  SCK      pin 13 (UNO, Nano, Mini), pin 52 (Mega), pin SCK (Leonardo), pin 15 (Pro micro)

*/

#include <ELEMYO.h>

#define   CSpin         10
#define   sensorInPin   A0     // analog input pin that the sensor is attached to
short timePeriod = 3;          // frequency of signal update (time in ms)

//-------- for exponential moving average method ----------------------------------------------------------------
float MA[3];                 //array for MA method: MA[0] - EMA, MA[1] - DMA, MA[2] - TMA
float alpha = 0.93;          // smoothing constant for EMA method
//---------------------------------------------------------------------------------------------------------------

int signalValue = 0;          // value read from the sensor
int siganlReference = 516;    // reference of signal, 2.5 V for MYO, MYO-kit, BPM, BPM-kit
//int siganlReference = 369;  // reference of signal, 1.8 V for MH-BPS101 and MH-BPS102

ELEMYO MyoSensor(CSpin);

void setup() {
  Serial.begin(115200);            // initialize serial communications at 115200 bps
  MyoSensor.gain(x1);              // initial value of gain
  pinMode(sensorInPin, INPUT);     // initialize sensorInPin

  for(int i=0; i<3; i++)
    MA[i] = siganlReference;
}

void loop() {
  // read the analog in value:
  signalValue = analogRead(A0);

  // rectification of the signal
  if (signalValue < siganlReference)
     signalValue = siganlReference*2 - signalValue;

  //--triple exponential moving average-----------
  MA[0] = (1-alpha)*signalValue + alpha*MA[0];
  for(int i=1; i<3; i++)
  {
    MA[i] = (1-alpha)*MA[i-1] + alpha*MA[i];
  }
  //----------------------------------------------
    
  // print the rectification of the signal to the Serial Monitor:
  Serial.print(signalValue);
  Serial.print(" ");
  // print the triple exponential moving average of the signal to the Serial Monitor:
  Serial.println(MA[2]);
  
  // wait before the next loop
  delay(timePeriod);
}
