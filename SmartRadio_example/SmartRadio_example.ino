#include <SmartTransceiver.h>
#include <nRF24L01.h>
#include <SPI.h>

SmartTransceiver radio;
int count = 0;


void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);
  Serial.println("setup(): Starting SmartTranceiver...");
    
  radio.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available() > 0){
    //radio.powerUp();  //  DELETE?
    String messageToSend = Serial.readString();
    messageToSend.trim();                            //Get rid of carriage returns and strip spaces at beginning and end
    radio.send(messageToSend);
  }
  
  radio.read();

  if(radio.messageReady()){
    Serial.println("\nReceived: " +radio.getMessage() + "\n");
    radio.clearMessage();
  }
  /**
  radio.send("{\"speed\": 0, \"pos\":0}");
  delay(5500);
  radio.send("{\"pos\":180}");
  delay(5500);
  radio.send("{\"pos\":90}");
  
  delay(5500);
  
  count += 3;
  Serial.print(count);
  radio.send("{\"sendVcc\":0}");
  delay(1000);**/
}
