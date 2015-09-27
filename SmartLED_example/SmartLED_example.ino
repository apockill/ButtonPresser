#include <SmartLED.h>

/*
RGB LED Pin Layout

   -----|\   RED
--------| \  GROUND
  ------| /  GREEN
   -----|/   BLUE
 */
//PINS
int redPin   = 3;
int greenPin = 5;
int bluePin  = 6;

//OTHER
SmartLED LED(redPin, greenPin, bluePin);    //SETUP SMART LED


void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);   //Super important. Makes Serial.parseFloat() not wait a long time after an integer has ended.
}

void loop() {
  while(Serial.available() > 0){
    String var = Serial.readStringUntil(':');
    int    val = (int) Serial.parseFloat();

    if(var.equals("r")){
      LED.setRGB(val, -1, -1);
      Serial.print("r:"); 
      Serial.println(val);
    }
    
    if(var.equals("g")){
      LED.setRGB(-1, val, -1);
      Serial.print("g:"); 
      Serial.println(val);
    }
    
    if(var.equals("b")){
      LED.setRGB(-1, -1, val);
      Serial.print("b:"); 
      Serial.println(val);
    }
    
    if(var.equals("m")){
      LED.setMode(val);
      Serial.print("m:"); 
      Serial.println(val);
    }
  }
  
  //PERFORM ACTIONS
  LED.update();

}
