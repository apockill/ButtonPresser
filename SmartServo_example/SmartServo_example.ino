#include <SmartServo.h>
#include <Servo.h>

/**
Orange: Pin ?
Red:    5V      
Brown:  Ground    (or to the powerPIN location)

NOTES:
  High torque servo:
    min: 71
    max: 618
    
  Low torque servo:
    min: 120
    max: 442
**/

SmartServo servo(4, 2, 0);  //What these input mean: servo(POWER pin, PWM pin, servo ANALOG pin)


void setup(){
  Serial.begin(9600);
  Serial.println(F("Starting Servo..."));

  //  SET SERVO DEFAULT VALUES 
  servo.init();
  //servo.calibrate(30);
  servo.setAnalogMinMax(71, 618);  // or use servo.calibrate(??);
  servo.setRotMinMax( 20, 160);
}

void loop(){ 
  if(Serial.available()){
    int moveTo = Serial.readString().toInt();
    
    Serial.print(F("Move To: "));
    Serial.print(moveTo);
    
    servo.on();
    delay(100);
    
    servo.move(moveTo);
    delay(500);
    Serial.print(F("\tAnalog Map: "));
    Serial.println(String(servo.getPosition()));
    servo.off();
 
  }
}
