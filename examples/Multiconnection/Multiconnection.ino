// This example demonstrates working with an array of sensors
// 2018-04-18 by ELEMYO (https://github.com/ELEMYO/Elemyo-library)
//
// Changelog:
//     2020-02-24 - initial release
//     2020-04-22 - BandStop filter added

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

#define   CSpin1         10
#define   CSpin2         9
#define   CSpin3         8
#define   CSpin4         7
#define   CSpin5         6
#define   CSpin6         5

// analog input pins that the sensors are attached to:
#define   sensorInPin1   A0     
#define   sensorInPin2   A1    
#define   sensorInPin3   A2    
#define   sensorInPin4   A3     
#define   sensorInPin5   A4     
#define   sensorInPin6   A5     

int sensorValue1 = 0;           // value read from the sensor 1
int sensorValue2 = 0;           // value read from the sensor 2
int sensorValue3 = 0;           // value read from the sensor 3
int sensorValue4 = 0;           // value read from the sensor 4
int sensorValue5 = 0;           // value read from the sensor 5
int sensorValue6 = 0;           // value read from the sensor 6

// sensors initialization:
ELEMYO MyoSensor1(CSpin1);
ELEMYO MyoSensor2(CSpin2);
ELEMYO MyoSensor3(CSpin3);
ELEMYO MyoSensor4(CSpin4);
ELEMYO MyoSensor5(CSpin5);
ELEMYO MyoSensor6(CSpin6);

void setup() {
  
  // initialize serial communications at 115200 bps:
  Serial.begin(115200);
  
  // initial value of gain: x1; x2; x3; x4; x5; x8; x10; x16; x32
  MyoSensor1.gain(x1);
  MyoSensor2.gain(x1);
  MyoSensor3.gain(x1);
  MyoSensor4.gain(x1);
  MyoSensor5.gain(x1);
  MyoSensor6.gain(x1);
  
  // initialisation of sensors input pins:
  pinMode(sensorInPin1, INPUT);
  pinMode(sensorInPin2, INPUT);
  pinMode(sensorInPin3, INPUT);
  pinMode(sensorInPin4, INPUT);
  pinMode(sensorInPin5, INPUT);
  pinMode(sensorInPin6, INPUT);
  
}

void loop() {

  // read the analog in value:
  sensorValue1 = analogRead(sensorInPin1);
  sensorValue2 = analogRead(sensorInPin2);
  sensorValue3 = analogRead(sensorInPin3);
  sensorValue4 = analogRead(sensorInPin4);
  sensorValue5 = analogRead(sensorInPin5);
  sensorValue6 = analogRead(sensorInPin6);
  
  // notch 50 Hz filter with band window 4 Hz
  sensorValue1 = MyoSensor1.BandStop(sensorValue1, 50, 4);
  sensorValue2 = MyoSensor2.BandStop(sensorValue2, 50, 4);
  sensorValue3 = MyoSensor3.BandStop(sensorValue3, 50, 4);
  sensorValue4 = MyoSensor4.BandStop(sensorValue4, 50, 4);
  sensorValue5 = MyoSensor5.BandStop(sensorValue5, 50, 4);
  sensorValue6 = MyoSensor6.BandStop(sensorValue6, 50, 4);

  // notch 100 Hz (one of 50 Hz mode) filter with band window 6 Hz
  sensorValue1 = MyoSensor1.BandStop(sensorValue1, 100, 6);
  sensorValue2 = MyoSensor2.BandStop(sensorValue2, 100, 6);
  sensorValue3 = MyoSensor3.BandStop(sensorValue3, 100, 6);
  sensorValue4 = MyoSensor4.BandStop(sensorValue4, 100, 6);
  sensorValue5 = MyoSensor5.BandStop(sensorValue5, 100, 6);
  sensorValue6 = MyoSensor6.BandStop(sensorValue6, 100, 6);
  
  Serial.print(sensorValue1+500);
  Serial.print(" ");
  Serial.print(sensorValue2+1000);
  Serial.print(" ");
  Serial.print(sensorValue3+1500);
  Serial.print(" ");
  Serial.print(sensorValue4+2000);
  Serial.print(" ");
  Serial.print(sensorValue5+2500);
  Serial.print(" ");
  Serial.println(sensorValue6+3000);

  // wait before the next loop
  delay(0);
}
