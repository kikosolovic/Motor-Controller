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

  if (!getRadioData()){
    wdt_reset();
    return;};
//1 joy
//2 watch
//3 steer
  switch (packet.header.controllerID) {
    case 1: {
      if (packet.header.dataLen == sizeof(joyData)) {
        joyData data;
        memcpy(&data, packet.payload, sizeof(data));
        joystickControllerProcedure(data);
      }
      break;
      }
       case 2: {
      if (packet.header.dataLen == sizeof(watchData)) {
        watchData data;
        memcpy(&data, packet.payload, sizeof(data));
        watchControllerProcedure(data);
      }
      break;
      }
          case 3: {
      if (packet.header.dataLen == sizeof(steerData)) {
        steerData data;
        memcpy(&data, packet.payload, sizeof(data));
        steerControllerProcedure(data);
      }
      break;
      }
    }


  
  
wdt_reset();
}










//----------------------------------------------------------- procedures










void joystickControllerProcedure(joyData data){
//      Serial.println(String(data.x) + "-x " + String(data.y) + "-y /toggle -" + String(data.sw)+ "/// pot  "+ String(data.pot) + "key: " + String(data.key));
    
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
data.sw =0;}

void watchControllerProcedure(watchData data){
//        Serial.println(String(data.btn1) + " -btn1 " + String(data.btn2) + " -btn2 " + String(data.btn3) + " -btn3 ");

        if (data.btn1){ // left
          continuousTurn(300);
          }
        else if (data.btn2) { //right
          continuousTurn(800);
          } 
        if (data.btn3) { // turn on engine
          if (!motorOn){
            throttle(200);
            motorOn = true;
            }
           else{
            throttle(0);
            motorOn = false;}
          }

        if (data.btn1 == 0 and data.btn2 == 0 and data.btn3 == 0){
            continuousTurn(503);
          }

        
  }
void steerControllerProcedure(steerData data){
//        Serial.println(String(data.throttle) + "-throttle " + String(data.steerAngle) + "-s angle "  + String(data.btn1) + "-btn1 "  + String(data.btn2) + "-btn2 "  + String(data.btn3) + "-btn3 " );


          turn(map(data.steerAngle,0,1023,705, 2347.5));
          throttle(data.throttle);


          if (data.btn1){ //CLock
            }
          if (data.btn2){} // throttle reverse on
          if (data.btn3){} //
        

        
  }
