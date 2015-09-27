#include <Servo.h>
#include <SPI.h>           //For SmartTransceiver
#include <nRF24L01.h>       //For SmartTransceiver
#include <ArduinoJson.h>    //For JSON handling (within sketch)
#include <SmartServo.h>
#include <SmartLED.h>
#include <SmartTransceiver.h>

/**
JSON VARIABLES DOCUMENTATION
  LED:
    r: red
    g: green
    b: blue
    m: mode  (0-off, 1-on, 2-pulse)
    pulseDelay: Speed of pulse
    
  servo:
    pos: Position (0-180 degrees)
    speed: The ms per degree (less is faster)
    gPos: If this is true, that means position has been requested via radio. This will trigger a one time radio.send() of the servo's current position.
    
  other:
    gVcc: Sends the current voltage of the arduino system via radio
**/


//  Variables for Pins
const int redPin   = 3;
const int greenPin = 5;
const int bluePin  = 6;
const int IRLEDPin = 2;

//  Other Variables
int servoCount = 2;        //  How many servos does this buttonPresser support

//  SET UP OBJECTS
SmartTransceiver   radio;
SmartServo         *servos[2];
SmartLED           LED(redPin, greenPin, bluePin);



/////////////////////////////////////  MAIN FUNCTIONS  /////////////////////////////////////
void setup(void){  
  Serial.begin(9600);
  //pinMode(IRLEDPin, OUTPUT);  //  SET UP INFRA RED PIN
  radio.init();
  
  
  //  SET SERVO DEFAULT VALUES
  servos[0] = new SmartServo(4,2,0);
  servos[1] = new SmartServo(8,7,1);
  for(int i = 0; i < servoCount; i += 1){
    servos[i]->init();
    servos[i]->setAnalogMinMax(71, 618);
    servos[i]->setRotMinMax(20, 160);
  }   
  
  
  //  UPDATE LED VALUES
  LED.setRGB(50, 50, 50);      //  Set starting values of LED's
  LED.setMode(0);              //  Resets the mode, and in doing so sends a refresh command, so even if mode didn't change, the colors might have.
}


void loop(void){
  //  CHECK FOR RADIO PACKETS AND ADD THEM TO CURRENTMESSAGE STRING
  radio.read();  
  
  
  //PROCESS RADIO MESSAGES THAT ARE FINISHED BEING READ
  if(radio.messageReady()){
    StaticJsonBuffer<200> tempBuffer;   //  CREATE TEMP BUFFER FOR PARSING JSONS. MEM WILL FREE ONCE SCOPE ENDS
    JsonObject& newJson = tempBuffer.parseObject((char*) String(radio.getMessage()).c_str());  //  PARSE JSON
         
    
    //IF THE RECEIVED MESSAGE WAS A VALID JSON, THEN CONTINUE
    if(newJson.success()){                          //  PROCESS JSON          
            
      //  SET ALL LED STUFF
      if(newJson.containsKey("r")){                 //  Change LED "red" setting
        LED.setRGB((int) newJson["r"], -1, -1);
      }
      
      if(newJson.containsKey("g")){                 //  Change LED "green" setting 
        LED.setRGB(-1, (int) newJson["g"], -1);
      }
      
      if(newJson.containsKey("b")){                 //  Change LED "blue" setting
        LED.setRGB(-1, -1, (int) newJson["b"]);
      }
      
      if(newJson.containsKey("m")){                 //  Change LED "mode" setting 
        LED.setMode((int) newJson["m"]);
      }
      
      
      //  SET ALL SERVO STUFF
      if(newJson.containsKey("servo")){                                 //  If a servo is receiving a command, the servo # must be specified in the "servo" variable in the JSON or else nothing will be done.
        int requestedServo = (int) newJson["servo"];
        
        if(newJson.containsKey("speed")){                               //  If there has been a change in the servo's speed setting, then change it before doing anything else with the servo
          servos[requestedServo]->setSpeed((int) newJson["speed"]);
        }      
        
        if(newJson.containsKey("pos")){                                 //  Move the requested servo
          servos[requestedServo]->move((int) newJson["pos"]);  
          delay(500);                                                   //  Give time for the servo to get to the end position before cutting power
          servos[requestedServo]->off();
        }
        
        if(newJson.containsKey("gPos")){                                //  Send position information if it was requested
          servos[requestedServo]->power();
          delay(15);                                                    //  For some reason without this small delay, servo.GetPosition gives erratic results (OPTIMIZED) 15ms
          radio.send(String(servos[requestedServo]->getPosition()));    //  DELETE (it uses String() which is memory intensive)
          delay(10);
          servos[0]->off();
        }
      }

      
      //  PERFORM MISC COMMANDS THAT MAY HAVE BEEN REQUESTED
      if(newJson.containsKey("gVcc")){              //  If the radio has requested the current voltage of the buttonpusher battery
        radio.send(String(readVcc()));
      }
     
     
      //  FINISH AND CLEAN UP      
      radio.send("Message Processed");
      radio.clearMessage();
      
    }else{radio.send("ERROR NOT A JSON");}
  }


  //PERFORM ACTIONS
  LED.update();
  //int code[] = {2016,240,6620,260,52360,220,34268,320,21644,380,40540,6280,3340,820,880,780,900,780,860,800,860,800,3400,780,3400,780,860,800};
  //sendIRCode(code);
}




/////////////////////////////////////  INFRA-RED FUNCTIONS  /////////////////////////////////////
void pulseIR(long microsecs) {
  while (microsecs > 0) {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
   digitalWrite(IRLEDPin, HIGH);  // this takes about 3 microseconds to happen
   delayMicroseconds(10);         // hang out for 10 microseconds
   digitalWrite(IRLEDPin, LOW);   // this also takes about 3 microseconds
   delayMicroseconds(10);         // hang out for 10 microseconds

   // so 26 microseconds altogether
   microsecs -= 26;
  }
}

void sendIRCode(int sendCode[]) {;
  //int sendCode[] = {2016,240,6620,260,52360,220,34268,320,21644,380,40540,6280,3340,820,880,780,900,780,860,800,860,800,3400,780,3400,780,860,800};
  for(int i = 0; i < sizeof(sendCode) / sizeof(int); i += 2){
    delayMicroseconds(sendCode[i]);
    pulseIR(sendCode[i+1]);
  }
}




/////////////////////////////////////  OTHER  /////////////////////////////////////
long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(4); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}
