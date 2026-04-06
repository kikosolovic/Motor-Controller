 #include <SPI.h>
#include "radio.h"
#include  "servo.h"
#include "esc.h"
#include "compass.h"
#include "compassLock.h"
#include "gps.h"
#include <avr/wdt.h>




void setup() {
    wdt_disable(); 
  Serial.begin(9600);
  initQMC5883P();
    initServo();
  initRadio();
  initEsc();
//initGps();


  wdt_enable(WDTO_4S); 
  Serial.println("ready");
  
}


bool normalMode = true;
bool joystickThrustMode = false;
bool settings = false;


void loop() {

  getRadioData(); //saved in data.
  if (data.key){

  switch (data.key){
    case 'A':{
      joystickThrustMode =  !joystickThrustMode;
      normalMode = !normalMode;
    break;
      }


    case 'B': {
      center();
      break;
      }
//testing peripherals
    case 'C':{
      
      calibrateCompass();
      }

     case 'D': {
      Serial.println("Compass test:");
      Serial.println("heading: " + String(SmoothAzimuth()) + "degrees") ;
      delay(1000);
      Serial.println("Gps test -> current nmea sentence:");
      Serial.println(gpsTest());
      delay(1000);
      Serial.println("Radio test:");
      Serial.println(String(data.x) + "-x " + String(data.y) + "-y /toggle -" + String(data.sw)+ "/// pot  "+ String(data.pot) + " key: " + String(data.key));
//      delay(1000);
//      Serial.println("Servo test:");
//      //dokoncit servo movement bez servo lib
//      delay(1000);
//      Serial.println("Esc test:");
//      Serial.println();
//      delay(1000);
      break;
      }
      
    case 'E':{   
      if (CLEnabled){
              cInterval = constrain(cInterval - 1, 0, 25);
              
      }
      else{
              servoInterval = constrain(servoInterval - 1,0,10);
        }
//        Serial.println(String(cInterval)   + " " + String(servoInterval) );

    break;}

    case 'F':
    { 

      if (CLEnabled){
        
        cInterval = constrain(cInterval + 1, 0, 25);
      }
      else{
              servoInterval = constrain(servoInterval + 1,0,10);
        }

//                Serial.println(String(cInterval)   + " " + String(servoInterval) );
      break;}
    

    

}}

if (data.sw){
    CLEnabled = !CLEnabled;
}
if (normalMode)
    {
      throttle(data.pot);
      continuousTurn(data.x);
      

      }
if (joystickThrustMode){
  throttleJoy(data.y);
  continuousTurn(data.x);
  }


data.key = 0;
data.sw =0;
wdt_reset();
}
