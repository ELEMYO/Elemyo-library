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

#define   CSpin         10      // chip select pin connected to MYO sensor
#define   EMG_PIN       A0      // analog pin connected to MYO sensor
#define   SERVO_PIN     5       // PWM pin connected to servo
#define   EMG_MAX       200     // Adjust based on your muscle signal

ELEMYO MyoSensor(CSpin);      // create ELEMYO object to work with signal
Servo myservo;                // create servo object to control a servo

void setup() {
  Serial.begin(115200);           // initialize serial communications at 115200 bps
  MyoSensor.begin();
  MyoSensor.gain(x2);             // initial value of gain
  pinMode(EMG_PIN, INPUT);        // initialize sensorInPin
  myservo.attach(SERVO_PIN);      // attaches the servo on pin 5 to the servo object
}

void loop() {
  int signalValue = analogRead(EMG_PIN);                // read the analog in value:
  signalValue = MyoSensor.BandStop(signalValue, 50, 4);     // notch 50 Hz filter with band window 4 Hz.  
  signalValue = MyoSensor.BandStop(signalValue, 100, 6);    // notch 100 Hz (one of 50 Hz mode) filter with band window 6 Hz

  int signalValueMA = MyoSensor.movingAverage(signalValue,  0.98); // moving average transformation with 0.98 smoothing constant

  Serial.print(signalValue - 504);                          // print the signal to the Serial Monitor:
  Serial.print(" ");
  Serial.println(signalValueMA);    // print the signal value after moving average transformation to the Serial Monitor:

  int val = map(signalValueMA, 0, EMG_MAX, 0, 180);      // scale signalValueMA to use it with the servo (value between 0 and 180)
  myservo.write(val);                                 // sets the servo position according to the scaled value
  
  delay(1);      // wait before the next loop
}
