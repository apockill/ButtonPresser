/*
  SmartServo.h - Library for controlling servos that have analog output and a pin that controls power entering a servo
  Created by Alex Thiel, 5/26/2015
*/
#include <Arduino.h>		//  Must be added to any header file
#include <Servo.h>

#ifndef SmartServo_h		//  This prevents problems if someone accidentally #include's this library twice
#define SmartServo_h

 

class SmartServo{
	public:
		SmartServo(int powerPin_, int controlPin_, int analogPin_); 
		void init();												//All it does here is set up the powerPin as an output. May work without...

		//void calibrate(int tries);
		void move(int moveTo);						 
		void on();													//  Send power and attach servo
		void off();													//  Detach servo and cut power
		void power();												//  Power the servo
		void attach();												//  Attach the servo
 

		int getPosition();

		//  SETTINGS
		void setAnalogMinMax(int analogMin_, int analogMax_);		
		void setRotMinMax(	 int servoMin_,  int servoMax_);		
		void setSpeed(int _speed);

		 


		 
	private:
		Servo servo;

		//  PINS
		int powerPin;		//  Controls a relay that sends power to the servo
		int controlPin;		//  The pin where PWM commands are sent to the servo
		int analogPin;		//  This pin returns an analog value between analogMin and analogMax, from the potentiometer within the servo

		//  CONSTANTS
		int servoMin;		//  The min degree that the servo can turn (usually 0)
		int servoMax;		//  The max degree that the servo can turn (usually 180)
		int analogMax;		//  The value returned by the analog wire when servo is at Max
		int analogMin;		//  Same as above, but when servo is at Min

		int pos;			//  Used to keep track of where the servo is without ever having to do servo.read() calls, which take more memory
		int speed;			//  Used to keep track of what speed the servo should go at. 0 is fastest.
};

#endif
