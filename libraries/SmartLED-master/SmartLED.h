/*
  SmartLED.h - Library for controlling an RGB LED for different modes (blink, pulse, on, off, etc.)

  I used uint8_t as often as possible to minimize memory usage on the arduino. 

  Created by Alex Thiel, 5/26/2015


   -----|\   RED
--------| \  GROUND
  ------| /  GREEN
   -----|/   BLUE
*/
#include <Arduino.h>	//  Must be added to any header file

#ifndef SmartLED_h	//  This prevents problems if someone accidentally #include's this library twice
#define SmartLED_h

 
class SmartLED{
	public:
		SmartLED(	 int rPin_, int gPin_, int bPin_); 
		void write(	 int rVal_, int gVal_, int bVal_); 	//  Should be private? This is a quickfunction to actually send the commands to the LED

		void setRGB( int rVal_, int gVal_, int bVal_);

		void setMode(int mode_);						//  Change the mode (on, off, pulse, blink, etc.) The LED only reacts when set() is called
		void update();									//	Performs all changes to the LED
		
		 
	private:
		//  RGB PINS
		uint8_t rPin;					//  CHANGE THESE TO #INCLUDE's EVENTUALLY
		uint8_t gPin;
		uint8_t bPin;

		//  CURRENT COLOR OF EACH LED  (0 - LEDMax)
		uint8_t rVal		 	= 50;	
		uint8_t gVal		 	= 50;
		uint8_t bVal		 	= 50;

		//  MANAGING ACTIONS
		uint8_t mode		 	= 0;	//  The current status of the LED (0: OFF, 1: ON, 2: BLINK, 3: PULSE) 
		uint8_t maxVal		 	= 50;	//  Max birghtness allowed
		bool    actionComplete	= false;//  Once this is true, setLED() will not do anything until it is false again. This saves useless CPU usage.

		//  PULSE MODE
		int     direction 		= 1;	//  Used during pulse to determine if the pulse is getting brighter or darker at any given moment
		uint8_t pulseDelay		= 30;	//  How many milliseconds in between changing the LED during pulse mode. 											This may also apply to blink mode?
		long    lastPulse	 	= 0;	//  The millis() time when the LED was last changed (usually 15ms, to control the pulse) 
		 
};

#endif