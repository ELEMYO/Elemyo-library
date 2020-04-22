//Example of EMG signal using for control
//Example shows how to detect short and long muscle contractions
//  2018-04-18 by ELEMYO (https://github.com/ELEMYO/Elemyo-library)
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


Wiring the ELEMYO sensor to an Arduino UNO

ELEMYO -->  UNO
  +        5V
  -        GND
  S        analog pin A0 (or other analog input)
  СS       diginal pin 10  (SS pin)
  SDI      diginal pin 11  (MOSI pin)
  CLK      diginal pin 13  (SCK pin)


*/

#include <ELEMYO.h>
#include <EEPROM.h>

#define   CSpin         10
#define   sensorInPin   A0     // analog input pin that the sensor is attached to


// these constants won't change:
short thresholdStart = 150; // threshold value to decide the begining of EMG signal
short thresholdEnd = 50;   // threshold value to decide the end of EMG signal
short timeTrigger = 500;    // time trigger for two different actions (time in ms).

// these variables will change:
short EMG_Amplitude = 0; // variable used to store EMG amplitude
long EMG_TimeStart = 0;  // variable used to store EMG starting time
long EMG_Time = 0;       // variable used to store time period between EMG start and end (time in ms)
boolean flag = false;        // variable used to store the last EMG status

ELEMYO MyoSensor(CSpin);

void setup() { 
  Serial.begin(115200);          // initialize serial communications at 115200 bps
  MyoSensor.gain(x1);            // Initial value of gain
  pinMode(sensorInPin, INPUT);   // init sensorInPin

  Serial.println("Type 1 - short muscle contraction");
  Serial.println("Type 2 - long muscle contraction");
}

void loop() {

  //mean EMG amplitude calculating for 200 measurements
  EMG_Amplitude = getMaxAmplitude(200);

  // if it is new start of EMG signal and the EMG amplitude is greater than the given threshold:
  if (!flag && EMG_Amplitude > thresholdStart)
  {
    //getting time of EMG start
    EMG_TimeStart = millis();
    
    //storing that the EMG started
    flag = true;
  }

  // if EMG signal have been started and the EMG amplitude is bellow than the given threshold:
  if (flag && EMG_Amplitude < thresholdEnd)
  {
    //calculation of time period between EMG start and end
    EMG_Time = millis() - EMG_TimeStart;
    //storing that the EMG ended
    flag = 0;
  }

  //if EMG signal ended and EMS signal time less the 500 ms
  if (!flag && EMG_Time <= timeTrigger && EMG_Time !=0)
  {
    Serial.println("Type 1");
    EMG_Time = 0;
  }

  //if EMG signal ended and EMS signal time more the 500 ms
  if (!flag && EMG_Time > timeTrigger)
  { 
    Serial.println("Type 2");
    EMG_Time = 0;
  }
}

short getMaxAmplitude(short N)
{
  short MAX = 0;
  short MIN = 1024;
  for (short i = 0; i < N; i++)
  {
    short temp = MyoSensor.BandStop(analogRead(sensorInPin), 50, 4);
    temp = MyoSensor.BandStop(temp, 100, 6);
    if (temp > MAX)
        MAX = temp;
    if (temp < MIN)
        MIN = temp;
  }
  return MAX - MIN;
}
