/*
  SmartServo.cpp - Library for controlling servos that have analog output
  Created by Alex Thiel, 5/26/2015
*/

#include "SmartTransceiver.h"  //  Get access to the definitions in header file

//SmartTransceiver::RF24 radio(9,10);

SmartTransceiver::SmartTransceiver():radio(9,10){

	//RF24 radio = RF24(9,10);
}


void SmartTransceiver::init(){
	radio.begin();
	radio.begin();
	radio.setPALevel(RF24_PA_HIGH);
	radio.setRetries(15,15);
	radio.openReadingPipe(1, receiverAddress);
	radio.startListening();

	currentMessage.reserve(352);  //  Reserve enough memory to store an 11 packet message (assuming 32 bits per packet)
}



void SmartTransceiver::send(String theMessage){
	radio.stopListening();
	theMessage       = startMessage + theMessage + endMessage;

	float infoPerPacket= packetSize - placeHolder.length()*2;
	float packetsToSend = (float)theMessage.length() / ((float)infoPerPacket);


	//SEND PACKETSIZE BIT PACKETS UNTIL FULL MESSAGE IS SENT. LAST PACKET MAY NOT BE FULL PACKETSIZE
	for(float i = 0; i < packetsToSend; i++){  


		//FIND THE FROM/TO CHARACTERS OF THE STRING YOU WILL BE CUTTING
		float from = i*infoPerPacket;
		float to;
		if(packetsToSend-i >= 1){
			to = i*(infoPerPacket)+infoPerPacket;
		}else{
			to = (i*infoPerPacket) + (packetsToSend - i)*infoPerPacket;
		}


		//CUT STRING INTO APPROPRIATE SECTION AND SEND IT TO A CHARACTER ARRAY
		String packet = placeHolder + theMessage.substring(from,round(to)) + placeHolder;  //Get a packetSize bit section of the string
		char *packetCharArray = (char*) packet.c_str();


		//ATTEMPT TO SEND THE STRING, IF THERE IS A FAILURE ATTEMPT AGAIN. DO THIS AN X AMOUNT OF TIMES
		radio.powerUp();

		/**
	    Serial.print(F("Packet # "));//Split up prints to enable using F() and save sram, and avoid using memory hungry String(x) function.
	    Serial.print(i); 
	    Serial.print(F(", Size ")); 
	    Serial.print(to-from); 
	    Serial.print(F(" To: "));
	    Serial.print(to);
	    Serial.print(F(": \t"));
	    Serial.print(packetCharArray);**/

		radio.openWritingPipe(receiverAddress);
		if(radio.write(packetCharArray, to-from+placeHolder.length()*2)){  		//If transmition was successfull
			Serial.println(F("\tSUCCESS"));
		}else{                                      							//If transmition was unsuccessfull
			Serial.println(F("\tFAILURE")); 
		}
		radio.powerDown();
	}

	//RESET EVERYTHING SO THAT RECEIVER STILL WORKS
	radio.powerUp();
	radio.openReadingPipe(1, receiverAddress);
	radio.startListening();
}



void SmartTransceiver::read(){
	//if(messageReady()){  //If there is currently a message waiting to be read through getMessage(), then do not record any messages till then. This shouldn't happen, but just in case.
	//	return;
	//}

	if (radio.available()){
		char packet[packetSize + 1];
		boolean successfull = radio.read(packet, packetSize + 1);

		if(!successfull){Serial.println(F("Packet Reading Failure"));}  //If radio reading was not successful

		String currentPacket = String(packet);
		//Serial.println(currentPacket);
		currentMessage = currentMessage + currentPacket;
	} 
}



bool SmartTransceiver::messageReady(){
	return currentMessage.indexOf(endMessage) != -1;
}



String SmartTransceiver::getMessage(){
	//  Clean up message and send it back
	currentMessage.replace(placeHolder, "");
	currentMessage.replace(startMessage,"");
	currentMessage.replace(endMessage,  "");
	return currentMessage;
}

void SmartTransceiver::clearMessage(){
	currentMessage = "";
}
