//  2018-04-18 by ELEMYO (https://github.com/ELEMYO/Arduino)

#include "ELEMYO.h"
#include "Arduino.h"
#include "SPI.h"

//     INITIATION 

ELEMYO::ELEMYO(byte pin) {
	_cs = pin;				// choose the chip select pin
	pinMode(_cs,OUTPUT);			// set the pin as output
    	digitalWrite(_cs,HIGH);			// set the pin to default HIGH state
    	SPI.begin();				// initiate SPI

	BS[0].INITIAL();
	}	

//  PUBLIC  METHODS


void ELEMYO::gain(byte value) {
	digitalWrite(_cs,LOW);			// select CHIP
	SPI.transfer(GAIN);  			// send GAIN command
	SPI.transfer(value);			// transfer data
	digitalWrite(_cs,HIGH);			// end transmission
	}

void BANDSTOP::INITIAL(){

	//----initialization of values: x(n-1), x(n-2), y(n-1), y(n-2)--------
	for(int i =0; i < 2; i++)	
	{
		Y[i] = 0;
		X[i] = 0;
	}

	Y[2] = 0;
	//-------------------------------------------------------------------

	T = 0;		// initialization of time
	f = 0;		// initialization of notch frequency
	BW = 0;		// initialization of band stop window
}

int ELEMYO::BandStop (int sensorValue, float f, float BW)
{  

	//----- search for BandStop filter with "f" parameter, that has been created before---------------
	int i = 0;
	while(abs(BS[i].f - f) > 0.001 && (i < LengthBS))
		i++;	
	//------------------------------------------------------------------------------------------------

	//--if filter with notch frequency "f" have not call before, create BANDSTOP element for this "f"----
	if (i == LengthBS)
	{
		LengthBS += 1;
		BS = (BANDSTOP *)realloc(BS,LengthBS*sizeof(BANDSTOP));
		BS[i].INITIAL();
		BS[i].f = f;
	}
	//-----------------------------------------------------------------------------

	//----time start---------------------------------------------------------------
	if(BS[i].T == 0)
	{
		BS[i].T = micros();
		return sensorValue;
	}
	//-----------------------------------------------------------------------------

 	short DT = micros()-BS[i].T;			// time period from previous function call

	BS[i].T = micros();				// time of function call
   
	//---Calculation of filter coefficients------------------------------------------
	float r = 1 - 0.00000314*BW*DT;   
	float b1 = -2*cos(0.000006283*f*DT);
	float a1 = r*b1; 
	float K = (1+a1+r*r)/(2+b1);
	//--------------------------------------------------------------------------------
	
	
	BS[i].Y[2] = K*(sensorValue + b1*BS[i].X[1] + BS[i].X[0]) - a1*BS[i].Y[1] - r*r*BS[i].Y[0];	//The new signal value after filtering

	//---save these values: x(n-1), x(n-2), y(n-1), y(n-2)---------------------------
	BS[i].Y[0] = BS[i].Y[1];
	BS[i].Y[1] = BS[i].Y[2];
	BS[i].X[0] = BS[i].X[1];
	BS[i].X[1] = sensorValue;
	//--------------------------------------------------------------------------------
  
	return (int) BS[i].Y[2];
}

void LOWPASS::INITIAL(){

	//---initialization of values: x(n-1) ... x(n-6), y(n-1) ... y(n-6)---------------
	for (int i = 0; i < 6; i++)
	{
		Y[i] = 0;
		X[i] = 0;
	}

	Y[6] = 0;
	//---------------------------------------------------------------------------------

	T = 0;			// initialization of time
	f = 0;			// initialization of cutoff friquency
}

int ELEMYO::LowPass (int sensorValue, float f, int type)
{  

	//----- search for LowPass filter with "f" parameter, that has been created before---------------
	int i = 0;
	while(abs(LP[i].f - f) > 0.001 && (i < LengthLP))
		i++;	
	//------------------------------------------------------------------------------------------------


	//--if filter with notch frequency "f" have not call before, create LOWPASS element for this "f"----
	if (i == LengthLP)
	{
		LengthLP += 1;
		LP = (LOWPASS *) realloc(LP,LengthLP*sizeof(LOWPASS));
		LP[i].INITIAL();
		LP[i].f = f;
	}
	//---------------------------------------------------------------------------------------------------


	//----time start-----------------------------------------------------------------
	if(LP[i].T == 0)
	{
		LP[i].T = micros();
		return sensorValue;
	}
	//-------------------------------------------------------------------------------

	float a[3], b[3], Q;
	float A[6], A0[4], A1_0, A1_1, A2_0, A2_1, b0;
	short DT = micros()-LP[i].T;			// time period from previous function call
	LP[i].T = micros();				// time of function call 	

	float fs = 1000000.0/DT;
	float fs_2 = 4.0*fs*fs;		

	//---Calculation of filter coefficients------------------------------------------
	float wa = 2.0*fs*tan(3.1416*f/fs); 
	float wa_2 = wa*wa; 
	float a0;
	float q = 2.0*fs*wa;
	float a_, b_;

	switch (type){
	case 0:
		Q = 1;
		a[0] = 0.5176;
		a[1] = 1.4142;
		a[2] = 1.9319;
		b[0] = 1;
		b[1] = 1;
		b[2] = 1;
	case 1:
		Q = 0.0895;
		a[0] = 0.1553;
		a[1] = 0.4243;
		a[2] = 0.5796;
		b[0] = 1.0230;
		b[1] = 0.5900;
		b[2] = 0.1570;
	case 2:
		Q = 0.0614;
		a[0] = 0.1244;
		a[1] = 0.3398;
		a[2] = 0.4641;
		b[0] = 0.9907;
		b[1] = 0.5577;
		b[2] = 0.1247;
	}

	a_ = a[0]*q;
	b_ = b[0]*wa_2;

	a0 = fs_2 + a_ + b_;
	A1_0 = 2.0*(- fs_2 + b_)/a0;
	A1_1 = 1.0 - 2.0*a_/a0;

	b0 = Q*wa_2/a0;

	a_ = a[1]*q;
	b_ = b[1]*wa_2;

	a0 = fs_2 + a_ + b_;
	A2_0 = 2.0*(- fs_2 + b_)/a0;
	A2_1 = 1.0 - 2.0*a_/a0;

	b0 = wa_2*b0/a0;

	A0[0] = A1_0 + A2_0;
	A0[1] = A1_1 + A1_0*A2_0 + A2_1;
	A0[2] = A1_1*A2_0 + A1_0*A2_1;
	A0[3] = A1_1*A2_1;

	a_ = a[2]*q;
	b_ = b[2]*wa_2;

	a0 = fs_2 + a_ + b_;
	A1_0 = 2.0*(- fs_2 + b_)/a0;
	A1_1 = 1.0-2.0*a_/a0;

	b0 = wa_2*b0/a0;

	A[0] = A0[0] + A1_0;
	A[1] = A0[1] + A0[0]*A1_0 + A1_1;
	A[2] = A0[2] + A0[1]*A1_0 + A0[0]*A1_1;
	A[3] = A0[3] + A0[2]*A1_0 + A0[1]*A1_1;
	A[4] = A0[3]*A1_0 + A0[2]*A1_1;
	A[5] = A0[3]*A1_1;
	//--------------------------------------------------------------------------------	



	//The new signal value after filtering
	LP[i].Y[6] = b0*((float)sensorValue) + 6.0*b0*LP[i].X[5] + 15.0*b0*LP[i].X[4] + 20.0*b0*LP[i].X[3] + 15.0*b0*LP[i].X[2] + 6.0*b0*LP[i].X[1] + b0*LP[i].X[0] - A[0]*LP[i].Y[5] - A[1]*LP[i].Y[4] - A[2]*LP[i].Y[3] - A[3]*LP[i].Y[2]- A[4]*LP[i].Y[1]- A[5]*LP[i].Y[0];
	
	//---save these values: x(n-1) ... x(n-6), y(n-1) ... y(n-6)----------------------
	for (int j = 0; j < 6; j++)
		LP[i].Y[j] = LP[i].Y[j+1];

	for (int j = 0; j < 5; j++)
		LP[i].X[j] = LP[i].X[j+1];

	LP[i].X[5] = sensorValue;
	//--------------------------------------------------------------------------------

	if(fs > 2.1*f)
		return (int) LP[i].Y[6];
	if(LP[i].T == 0)
		return (int) LP[i].Y[6];
	
	Serial.print("ERROR IN FILTER MyoSensor.LowPass(...) : the loop frequency ( ");
	Serial.print(fs);
	Serial.print(" ) is smaller than 2.1* cutoff frequency ( 2.1*");
	Serial.print(f);
	Serial.println(" ).");
	delay(100000000);
	return 0;
}

void BANDPASS::INITIAL(){

	//---save these values: x(n-1) ... x(n-8), y(n-1) ... y(n-8)------------------------
	for(int i = 0; i < 8; i++)
	{
		Y[i] = 0;
		X[i] = 0;
	}

	Y[8] = 0;
	//----------------------------------------------------------------------------------

	T = 0;			// initialization of time
	fl = 0;			// initialization of lower cutoff friquency
	fh = 0;			// initialization of upper cutoff friquency
}

int ELEMYO::BandPass (int sensorValue, float fl, float fh, int type)
{  

	//----- search for BandPass filter with "f" parameter, that has been created before---------------
	int i = 0;
	while((abs(BP[i].fl-fl) + abs(BP[i].fh-fh))>0.001 && (i<LengthBP))
		i++;	
	//------------------------------------------------------------------------------------------------
	
	//--if filter with notch frequency "f" have not call before, create BANPASS element for this "f"----
	if (i == LengthBP)
	{
		LengthBP+=1;
		BP = (BANDPASS *) realloc(BP,LengthBP*sizeof(BANDPASS));
		BP[i].INITIAL();
		BP[i].fl = fl;
		BP[i].fh = fh;
	}
	//--------------------------------------------------------------------------------------------------

	//----time start-----------------------------------------------------------------
	if(BP[i].T == 0)
	{
		BP[i].T = micros();
		return sensorValue;
	}
	//-------------------------------------------------------------------------------

	float a[2], b[2], Q;

	short DT = micros()-BP[i].T;			// time period from previous function call
	BP[i].T = micros();				// time of function call 	

	float fs = 1000000.0/DT;			// sampling frequency	

	//---Calculation of filter coefficients------------------------------------------
	float A[8], A1[4], A2[4], b0;
	float wah = 2.0*fs*tan(3.1416*fh/fs);
	float wal = 2.0*fs*tan(3.1416*fl/fs); 
	float w = (wah-wal);
	float w0_2 = wah*wal;
	float w0_4 = w0_2*w0_2/fs/fs;
	float fsfs = 16.0*fs*fs;
	float a0;

	switch (type){
	case 0:
		Q = 1;
		a[0] = 0.7654;
		a[1] = 1.8478;
		b[0] = 1;
		b[1] = 1;
	case 1:
		Q = 0.3579;
		a[0] = 0.3507;
		a[1] = 0.8467;
		b[0] = 1.0635;
		b[1] = 0.3564;
	case 2:
		Q = 0.2456;
		a[0] = 0.2791;
		a[1] = 0.6737;
		b[0] = 0.9865;
		b[1] = 0.2794;
	}


	a0 = fsfs + 8.0*fs*a[0]*w + 4.0*(2.0*w0_2 + b[0]*w*w) + 2.0*a[0]*w0_2*w/fs + w0_4;
	A1[0] = (-4.0*fsfs - 16.0*fs*a[0]*w + 4.0*a[0]*w0_2*w/fs + 4.0*w0_4)/a0;
	A1[1] = (6.0*fsfs - 8.0*(2.0*w0_2 + b[0]*w*w) + 6.0*w0_4)/a0;
	A1[2] = (-4.0*fsfs + 16.0*fs*a[0]*w - 4.0*a[0]*w0_2*w/fs + 4.0*w0_4)/a0;
	A1[3] = (fsfs - 8.0*fs*a[0]*w + 4.0*(2.0*w0_2 + b[0]*w*w) - 2.0*a[0]*w0_2*w/fs + w0_4)/a0;

	b0 = w*w*w;
	b0 = b0/a0;

	a0 = fsfs + 8.0*fs*a[1]*w + 4.0*(2.0*w0_2 + b[1]*w*w) + 2.0*a[1]*w0_2*w/fs + w0_4;
	A2[0] = (-4.0*fsfs - 16.0*fs*a[1]*w + 4.0*a[1]*w0_2*w/fs + 4.0*w0_4)/a0;
	A2[1] = (6.0*fsfs - 8.0*(2.0*w0_2 + b[1]*w*w) + 6.0*w0_4)/a0;
	A2[2] = (-4.0*fsfs + 16.0*fs*a[1]*w - 4.0*a[1]*w0_2*w/fs + 4.0*w0_4)/a0;
	A2[3] = (fsfs - 8.0*fs*a[1]*w + 4.0*(2.0*w0_2 + b[1]*w*w) - 2.0*a[1]*w0_2*w/fs + w0_4)/a0;

	b0 = 16.0*Q*w*b0/a0;

	A[0] = A1[0] + A2[0];
	A[1] = A2[1] + A1[0]*A2[0] + A1[1];
	A[2] = A2[2] + A1[0]*A2[1] + A1[1]*A2[0] + A1[2];
	A[3] = A2[3] + A1[0]*A2[2] + A1[1]*A2[1] + A1[2]*A2[0] + A1[3];
	A[4] = A1[0]*A2[3] + A1[1]*A2[2] + A1[2]*A2[1] + A1[3]*A2[0];
	A[5] = A1[1]*A2[3] + A1[2]*A2[2] + A1[3]*A2[1];
	A[6] = A1[2]*A2[3] + A1[3]*A2[2];
	A[7] = A1[3]*A2[3];
	
	//----------------------------------------------------------------------------------

	//The new signal value after filtering
	BP[i].Y[8] = b0*((float)sensorValue) - 4.0*b0*BP[i].X[6] + 6.0*b0*BP[i].X[4] - 4.0*b0*BP[i].X[2] + b0*BP[i].X[0] - A[0]*BP[i].Y[7] - A[1]*BP[i].Y[6] - A[2]*BP[i].Y[5] - A[3]*BP[i].Y[4] - A[4]*BP[i].Y[3] - A[5]*BP[i].Y[2] - A[6]*BP[i].Y[1] - A[7]*BP[i].Y[0];
	
	//---save these values: x(n-1) ... x(n-8), y(n-1) ... y(n-8)------------------------
	for (int j = 0; j < 8; j++)
		BP[i].Y[j] = BP[i].Y[j+1];

	for (int j = 0; j < 7; j++)
		BP[i].X[j] = BP[i].X[j+1];

	BP[i].X[7] = sensorValue;
	//-------------------------------------------------------------------------------------

	if(fs > 2.1*fh)
		return (int) BP[i].Y[8];

	Serial.print("ERROR IN FILTER MyoSensor.BandPass(...) : the loop frequency ( ");
	Serial.print(fs);
	Serial.print(" ) is smaller than 2.1* upper cutoff frequency ( 2.1*");
	Serial.print(fh);
	Serial.println(" ).");
	delay(1000000000);
	return 0;
}
