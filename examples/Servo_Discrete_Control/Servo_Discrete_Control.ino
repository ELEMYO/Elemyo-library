// Controlling a servo position using EMG signal.
// 2020-06-22 by ELEMYO (https://github.com/ELEMYO/Elemyo-library)
//
// Changelog:
//     2020-06-22 - initial release

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

Sensor -->  Arduino
  +        5V
  -        GND
  S        analog pin A0 (or other analog input)
  СS       pin 10 (SS pin)
  MOSI     pin 11 (UNO, Nano, Mini), pin 51 (Mega), pin SI (Leonardo), pin 16 (Pro micro)
  SCK      pin 13 (UNO, Nano, Mini), pin 52 (Mega), pin SCK (Leonardo), pin 15 (Pro micro)

*/

#include <ELEMYO.h>
#include <Servo.h>

#define   CSpin         10     // chip select pin
#define   sensorInPin   A0     // analog input pin that the sensor is attached to

int signalReference = 524;    // reference of signal, 2.5 V for MYO, MYO-kit, BPM, BPM-kit
//int signalReference = 369;  // reference of signal, 1.8 V for MH-BPS101 and MH-BPS102
long timer = 0;               // time length of EMG signal

ELEMYO MyoSensor(CSpin);      // create ELEMYO object to work with signal
Servo myservo;                // create servo object to control a servo

void setup() {
  Serial.begin(115200);           // initialize serial communications at 115200 bps
  MyoSensor.gain(x2);             // initial value of gain
  pinMode(sensorInPin, INPUT);    // initialize sensorInPin
  myservo.attach(5);              // attaches the servo on pin 5 to the servo object

  Serial.println("Type 1 - short muscle contraction");
  Serial.println("Type 2 - long muscle contraction");
}

void loop() {
  int signalValue = analogRead(sensorInPin);                // read the analog in value:
  signalValue = MyoSensor.BandStop(signalValue, 50, 4);     // notch 50 Hz filter with band window 4 Hz.  
  signalValue = MyoSensor.BandStop(signalValue, 100, 6);    // notch 100 Hz (one of 50 Hz mode) filter with band window 6 Hz

  int signalValueMA = MyoSensor.movingAverage(signalValue, signalReference,  0.92); // moving average transformation with 0.92 smoothing constant

  // start timer if EMG amplitude is greater than the given threshold and it is new start of EMG signal
  if (signalValueMA >= 200 && timer == 0)
      timer = millis();

  // if EMG amplitude is bellow than the given threshold
  if (signalValueMA < 200 && timer != 0)
  {
    if ((millis() - timer) < 500) // if EMG time length less then 0.5 c
    {
      Serial.println("Type 1");
      myservo.write(10);
    }
    else                          // if EMG time length more then 0.5 c
    {
      Serial.println("Type 2"); 
      myservo.write(120);
    }
    timer = 0;
  }
  
  delay(1);      // wait before the next loop
}
