//  Firmware for Arduino microcontrollers for interaction with the sensors via ELEMYO_GUI
//  2024-12-01 by ELEMYO (https://github.com/ELEMYO/ELEMYO-GUI)
//
// Changelog:
//  2024-12-01 - initial release

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

#include "SPI.h"

int sensorInPin[] = {A0, A1, A2, A3, A4, A5};  // analog input pins that the sensors is attached to
int sensorsNumber = 1;                         // number of sensors attached to Arduino (max 6)
int CSpin[] = {10, 9, 8, 7, 6, 5};             // CS pins for gain selection
byte sensorGain[] = {B000, B001, B010, B011, B100, B101, B110, B111}; // sensor gain (x1; x2; x4; x5; x8; x10; x16; x32)

void set_gain(short sensor, byte gain_value); // set sensor gain

void setup() {
  Serial.begin(250000);   // initialize serial communications at 250000 bps
  SPI.begin();    // initialize SPI interface
  
  for (int i = 0; i < 6; i++)
  {
    pinMode(sensorInPin[i], INPUT);  // initialize input pins
    pinMode(CSpin[i], OUTPUT);  // initialize input pins
    digitalWrite(CSpin[i],HIGH);
  }
}

long Time = micros();
void loop() {
  byte buf[16];  // byte array of data
  int data = 0;
  
  // read the last sensor signal value
  for (int i = 0; i < sensorsNumber ; i++)
  {  
    data = analogRead(sensorInPin[i]); // read sensor signal value
    buf[2*i] = data & 255;
    buf[2*i+1] = (data >> 8)  & 255;
  }

  data = micros() - Time;
  Time = micros();
  
  buf[12] = data & 255;
  buf[13] = (data >> 8)  & 255;
  
  buf[14] = 0xFF;
  buf[15] = 0xFF; 
  
  Serial.write(buf, 16); // write to serial 16 separating bytes
  
  for (int i = 0; i < 16 ; i++)
    buf[i] = 0;

  // checking for input command from ELEMYO GUI
  if (Serial.available() > 0) {
    int data = Serial.read(); // read data from serial buffer
    if (data < 7)
      sensorsNumber = data;   // set sensors number
    else
      set_gain(int(data/10) - 1, sensorGain[int(data%10)]);
  }
}

void set_gain(short sensor, byte gain_value) {
  digitalWrite(CSpin[sensor],LOW);      // select sensor
  SPI.transfer(B01000000);              // send GAIN command
  SPI.transfer(gain_value);             // send GAIN value
  digitalWrite(CSpin[sensor],HIGH);     // end transmission
  }
