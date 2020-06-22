//  Reads an analog input on pin A0, prints the result to the Serial Monitor.
//  Graphical representation is available using Matlab Terminal.
//  2018-04-18 by ELEMYO (https://github.com/ELEMYO/Elemyo-library)
//
// Changelog:
//     2020-02-24 - initial release
//     2020-04-22 - BandStop filter added
//     2020-06-22 - Moving average added

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
int signalReference = 524;    // reference of signal, 2.5 V for MYO, MYO-kit, BPM, BPM-kit
//int signalReference = 369;  // reference of signal, 1.8 V for MH-BPS101 and MH-BPS102

ELEMYO MyoSensor(CSpin);

void setup() {
  Serial.begin(115200);         // initialize serial communications at 115200 bps:
  MyoSensor.gain(x1);           // initial value of gain: x1; x2; x4; x5; x8; x10; x16; x32
  pinMode(sensorInPin, INPUT);   // initialisation of sensorInPin
}

void loop() {
  int sensorValue = analogRead(sensorInPin);              // read an analog in value:
  sensorValue = MyoSensor.BandStop(sensorValue, 50, 4);   // notch 50 Hz filter with band window 4 Hz
  sensorValue = MyoSensor.BandStop(sensorValue, 100, 6);  // notch 100 Hz (one of 50 Hz mode) filter with band window 6 Hz
  
  // moving average transformation with 0.8 smoothing constant.
  int sensorValueMA = MyoSensor.movingAverage(sensorValue, signalReference,  0.8); 
  
  Serial.print(sensorValue);      // print the results to the Serial Monitor:
  Serial.print(" ");
  Serial.println(sensorValueMA + signalReference);    // print the results to the Serial Monitor:
  
  delay(1);   // wait before the next loop
}
