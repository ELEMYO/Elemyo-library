//  Reads an analog input on pin A0, prints the result to the Serial Monitor.
//  The sensor gain is controlled by the program.
//  Graphical representation is available using Elemyo_GUI, Python_GUI and Matlab_GUI.
//  2018-04-18 by ELEMYO (https://github.com/ELEMYO/Elemyo-library)
//
// Changelog:
//    2018-04-18 - initial release
//    2020-06-15 - loop time sending added
//    2022-05-26 - 6 sensors data channels were added

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
  S        analog pins A0, A1, A2, A3, A4, A5
  СS       pins        10, 9,  8,  7,  6,  5 (SS pins)
  MOSI     pin 11 (UNO, Nano, Mini), pin 51 (Mega), pin SI (Leonardo), pin 16 (Pro micro)
  SCK      pin 13 (UNO, Nano, Mini), pin 52 (Mega), pin SCK (Leonardo), pin 15 (Pro micro)

*/

#include <ELEMYO.h>

int CSpin[] = {10, 9, 8, 7, 6, 5};
int sensorInPin[] = {A0, A1, A2, A3, A4, A5};  // analog input pins that the sensors is attached to
int sensorsNumber = 1;                         // number of sensors attached to Arduino
int timePeriod = 100 + 310*sensorsNumber;      // signal sampling time period (in microseconds)
int sensorGain[] = {x1, x2, x4, x5, x8, x10, x16, x32}; // sensor gain coefficients
ELEMYO MyoSensor[6];

void setup() {
  Serial.begin(115200);              // initialize serial communications at 115200 bps
  for (int i = 0; i < 6; i++)
  {
    pinMode(sensorInPin[i], INPUT);  // initialize input pins
    MyoSensor[i] = ELEMYO(CSpin[i]); // initialize sensors and CS pins
    MyoSensor[i].gain(x1);           // initial sensor gain
  }
}

void loop() {
  long Time = micros(); 
  
  for (int i = 0; i < sensorsNumber - 1 ; i++)
  {
    // read and write to serial sensors signal value
    Serial.print(analogRead(sensorInPin[i])); 
    Serial.print(" ");
  }

  // read and write to serial the last sensor signal value
  Serial.println(analogRead(sensorInPin[sensorsNumber - 1])); 

  // checking for input command from ELEMENTO GUI
  if (Serial.available() > 0) {
    int data = Serial.read(); // read data from serial buffer
    if (data < 7){
      sensorsNumber = data;   // set sensors number
      timePeriod = 100 + 310*sensorsNumber; // change the signal sampling time period
    }
    else
      MyoSensor[int(data/10) - 1].gain(sensorGain[int(data%10)]); // set new sensor gain
  }

  // delay for align sampling time period
  delayMicroseconds(timePeriod - (micros() - Time) - 12);
}
