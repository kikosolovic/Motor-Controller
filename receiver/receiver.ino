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

uint8_t current_controller_id;

joyData data_joy;
watchData data_watch;
steerData data_steer;
bool NewPacket = false;

void loop() {

  NewPacket = getRadioData();

//1 joy
//2 watch
//3 steer
if (NewPacket){
  switch (packet.header.controllerID) {
    case 1: {
      if (packet.header.dataLen == sizeof(joyData)) {

        memcpy(&data_joy, packet.payload, sizeof(data_joy));
        current_controller_id = 1;
        

      }
      break;
      }
       case 2: {
      if (packet.header.dataLen == sizeof(watchData)) {
        
        memcpy(&data_watch, packet.payload, sizeof(data_watch));
        current_controller_id = 2;
        CLEnabled = true;

      }
      break;
      }
          case 3: {
      if (packet.header.dataLen == sizeof(steerData)) {

        memcpy(&data_steer, packet.payload, sizeof(data_steer));
        current_controller_id = 3;
        steerControllerProcedure();
      }
      break;
      }}}
    

switch (current_controller_id){
  case 1: {
    
    joystickControllerProcedure();
    break;
    }
      case 2: {
    
    watchControllerProcedure();
    break;
    }
//    case 3: {
//              steerControllerProcedure();
//              break;}
    
    
    
  
  }
wdt_reset();
}










//----------------------------------------------------------- procedures










void joystickControllerProcedure(){
//      Serial.println(String(data.x) + "-x " + String(data.y) + "-y /toggle -" + String(data.sw)+ "/// pot  "+ String(data.pot) + "key: " + String(data.key));

  if (data_joy.key){

  switch (data_joy.key){
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
      Serial.println(String(data_joy.x) + "-x " + String(data_joy.y) + "-y /toggle -" + String(data_joy.sw)+ "/// pot  "+ String(data_joy.pot) + " key: " + String(data_joy.key));
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

if (data_joy.sw){
    CLEnabled = !CLEnabled;
}
if (normalMode)
    {
      throttle(data_joy.pot);
      continuousTurn(data_joy.x);
      

      }
if (joystickThrustMode){
  throttleJoy(data_joy.y);
  continuousTurn(data_joy.x);
  }


data_joy.key = 0;
data_joy.sw = 0;}


uint8_t watchSpeed = 200;
void watchControllerProcedure(){
//        Serial.println(String(data_watch.btn1) + " -btn1 " + String(data_watch.btn2) + " -btn2 " + String(data_watch.btn3) + " -btn3 ");

        if (data_watch.btn3 and data_watch.btn2){
          watchSpeed = 100;
          throttle(watchSpeed);}
        else if (data_watch.btn3 and data_watch.btn1){
          watchSpeed = 200;
          throttle(watchSpeed);}

        else if (data_watch.btn1){ // left
          continuousTurn(397);
          }
        else if (data_watch.btn2) { //right
          continuousTurn(800);
          }

        else {
            continuousTurn(503);
          }

        if (data_watch.btn3){
          motorOn = !motorOn;
          throttle(motorOn ? watchSpeed : 0);
          data_watch.btn3 = 0;}



        
  }
bool steerReverseEnabled = false;
void steerControllerProcedure(){
//        Serial.println(String(data.throttle) + "-throttle " + String(data.steerAngle) + "-s angle "  + String(data.btn1) + "-btn1 "  + String(data.btn2) + "-btn2 "  + String(data.btn3) + "-btn3 " );


          turn(map(data_steer.steerAngle,0,1023,705, 2347.5));



          if (data_steer.btn1){ //CLock

            }
          if (data_steer.btn2){
            steerReverseEnabled = !steerReverseEnabled;
            } // throttle reverse on

          if (data_steer.btn3 == 1){
            CLEnabled = true;
           continuousTurn(503);
           
           }
           else if (data_steer.btn3 == 2){
            CLEnabled= false;}
            


            
          if (steerReverseEnabled){
            reverseThrottle(data_steer.throttle);
          }
          else{
            throttle(data_steer.throttle);
            }
            
        

        
  }
