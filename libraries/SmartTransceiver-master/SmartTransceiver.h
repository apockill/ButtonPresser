/*
  SmartTransceiver.h - Library for receiving and transmitting radio
  Created by Alex Thiel, 5/26/2015
*/
#include <Arduino.h>		//  Must be added to any header file
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>    //  REMOVE LATER (?)
#include <SPI.h>

#ifndef SmartTransceiver_h	//  This prevents problems if someone accidentally #include's this library twice

class SmartTransceiver{
	public:
		SmartTransceiver();
		void init();
		void send(String theMessage);
		void read();
		void clearMessage();
		boolean messageReady();	//	If all packets in a message have been received this returns true, and getMessage will not return blank
		String getMessage();

	private:
		//  THINGS FOR RECEIVER AND TRANSMITTER
		RF24 radio;
		const uint64_t receiverAddress 	= 0xABCDABCD71LL;	//  The radio pipe (there are six) for com to be sent and read from (I think)
		const int packetSize 			= 31;				//  The max amount the transmitter can send (for some reason... Should be 32.)
		const String startMessage 		= "+";				//  Marks the start of all packets. (no purpose yet...)
		const String endMessage 		= "-";				//  Marks the end of all packets, and receiever will zip it up and process it.
		const String placeHolder 		= "_";				//  The reciever will delete any of these characters when they are found. Useful for getting the receiver to accurately read stuff!

		//  THINGS FOR RECEIVER ONLY
		String currentMessage;		//  Where the current message is built.
};

//extern SmartTransceiver RF24;

#endif