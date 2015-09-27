/*
  SmartLED.cpp - Library for controlling an RGB LED for different modes (blink, pulse, on, off, etc.)
  Created by Alex Thiel, 5/26/2015
*/

#include "SmartLED.h"  //  Get access to the definitions in header file

SmartLED::SmartLED(int rPin_, int gPin_, int bPin_){
	rPin = rPin_;
	gPin = gPin_;
	bPin = bPin_;

}



void SmartLED::update(){
	//  Mode: 0 = Off, 1 = On, 2 = Pulse

	//QUIT FUNCTION IF COMMAND HAS ALREADY BEEN CARRIED OUT PREVIOUSLY
	if(actionComplete){return;}   
	Serial.println(mode);

	//  MODE 0: TURN LED OFF
	if(mode == 0){
		write(0,0,0);
		actionComplete = true;
		return;
	}


	//  MODE 1: TURN LED ON
	if(mode == 1){

		write(rVal, gVal, bVal);
		actionComplete = true;
		return;
	}


	//  MODE 2: PULSE LED
	long milli = millis();	//  Since millis is used twice, its more memory efficient to pull it only once.

	if(mode == 2 && milli - lastPulse >= pulseDelay){  //Pulse, and only pulse every 30 milliseconds
		lastPulse = milli;

		//  FLIP DIRECTIONS OF PULSE
		if(rVal <= abs(direction)	|| gVal <= abs(direction)	|| bVal <= abs(direction)	){	direction =  abs(direction);	}
		if(rVal >= maxVal			|| gVal >= maxVal 			|| bVal >= maxVal			){	direction = -abs(direction);	}

		//  SET PULSE
		rVal += direction;
		gVal += direction;
		bVal += direction;
		//Serial.println("RVal: " + String(rVal) + "  \t gVal: " + String(gVal) + "  \t bVal: " + String(bVal) + "  \t\t direction: " + direction + "   \tactionComplete: " + actionComplete + "  \tmode: " + mode + "\t millis: " + String(milli));
		write(rVal, gVal, bVal);
		return;
	}
}




void SmartLED::setMode(int mode_){	//  Change the mode (on, off, pulse, blink, etc.)
	mode = mode_;
	actionComplete = false;
}



void SmartLED::setRGB(int rVal_, int gVal_, int bVal_){
	//  TO CHANGE ONLY ONE COLOR AT A TIME, PUT -1 FOR THE OTHER COLORS, AND THEY WILL NOT BE CHANGED.
	
	if(rVal_ != -1){
		rVal = rVal_;
	}
	if(gVal_ != -1){
		gVal = gVal_;
	}
	if(bVal_ != -1){
		bVal = bVal_;
	}
	actionComplete = false;
}


void SmartLED::write(int rVal_, int gVal_, int bVal_){
	//Serial.println("rVal: " + String(rVal) + "\tgVal: " + String(gVal) + "\tbVal: " + String(bVal));  //  For debugging

	//  Ensures that the LED has a valid value to output
	rVal_	= constrain(rVal_, 0, maxVal);
	gVal_	= constrain(gVal_, 0, maxVal);
	bVal_	= constrain(bVal_, 0, maxVal);

	//  Actually write to the LED Pins
	analogWrite(rPin, rVal_);
	analogWrite(gPin, gVal_);
	analogWrite(bPin, bVal_); 
}