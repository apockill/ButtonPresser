/*
  SmartServo.cpp - Library for controlling servos that have analog output
  Created by Alex Thiel, 5/26/2015
*/
#include "SmartServo.h"  //  Get access to the definitions in header file

SmartServo::SmartServo(int powerPin_, int controlPin_, int analogPin_){

	powerPin 	= powerPin_;
	controlPin  = controlPin_;
	analogPin 	= analogPin_;

	//  SET DEFAULT VALUES (THESE CAN BE CHANGED WITH THE APPROPRIATE FUNCTIONS)
	analogMax   = 618;
	analogMin 	= 71;
	servoMax	= 180;
	servoMin	= 0;
	speed 		= 15;
}


void SmartServo::init(){
	pinMode(powerPin, OUTPUT);		
	pos = 90;
}


/**
void SmartServo::calibrate(int tries){
	int avgMax = 0;
	int avgMin = 0;
	servo.attach(controlPin);	
	servo.write((int) (servoMax - servoMin) / 2);	//  Center servo before beginning
	delay(100);

	 					//  Activate servo
	for(int i = 0; i < tries; i++){					//  Get i amount of samples
		move(servoMax, 10);					
		delay(500);
		avgMax   += analogRead(analogPin);			//  Record analogValue at servoMax rotation

		move(servoMin, 10);
		delay(500);
		avgMin   += analogRead(analogPin);			//  Record analogValue at servoMin rotation
		//Serial.println("avgMax: " + String(avgMax) + "\tavgMin: " + String(avgMin));	//  DEBUGGING
	}
	servo.write((int) (servoMax - servoMin) / 2);	//  Center servo, for aesthetics
	delay(500);
	servo.detach();									//  Deactivate servo so that it an be moved by hand

	analogMax = avgMax / tries;
	analogMin = avgMin / tries;
	//Serial.println(analogMax);
	//Serial.println(analogMin);
}**/



int SmartServo::getPosition(){
	return analogRead(analogPin);
	//return map(analogRead(analogPin), analogMin, analogMax, servoMin, servoMax);    //  Not as modular
}



void SmartServo::move(int moveTo){
	on();
	
	int sign = (moveTo - pos) / abs(moveTo - pos); 		//  Get sign of function (aka the direction of movement)

	moveTo = constrain(moveTo, servoMin, servoMax);  	//  Fix any mistakes that would break the servo

	for(; pos != moveTo; pos+= sign){					//  The private variable Pos is used to keep track of where the servo is. 

		servo.write(pos);
		delay(speed);
	}
}


void SmartServo::power(){
	digitalWrite(powerPin, HIGH);
}

void SmartServo::attach(){
	servo.attach(controlPin);
}

void SmartServo::on(){
	digitalWrite(powerPin, HIGH);
	delay(100);
	servo.attach(controlPin);
	delay(100);
}

void SmartServo::off(){
	//servo.detach();
	digitalWrite(powerPin, LOW);
}



void SmartServo::setAnalogMinMax(int analogMin_, int analogMax_){
	analogMin = analogMin_;
	analogMax = analogMax_;
}




void SmartServo::setRotMinMax(int servoMin_, int servoMax_){
	servoMin = servoMin_;
	servoMax = servoMax_;
}

void SmartServo::setSpeed(int _speed){
	speed = _speed;
}