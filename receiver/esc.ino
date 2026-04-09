#include <Servo.h>
#include "esc.h"
Servo esc;
int fanPin = 6;
int escPin = 3;


void initEsc(){
    esc.attach(escPin);

    esc.writeMicroseconds(2000);  // Full throttle (20ms)
    delay(50);                    // Shortest reliable pulse (~2.5 cycles)
    
    esc.writeMicroseconds(1000);  // Full brake (20ms)  
    delay(50);                    // Shortest reliable pulse 
  
    esc.writeMicroseconds(1500);  // Neutral/stop - stays here
    delay(150);

     initFan();

    }

void fanOn(){
  if (offfan){
digitalWrite(fanPin,LOW);
offfan = false;
    }

}
void fanOff(){
  if (!offfan){
      digitalWrite(fanPin, HIGH);
      offfan = true;}
    }

void throttleJoy(int x){
          int speed;
  if (x > 502 and x < 505){
    speed = 1500;}
  else{
    speed = map(x,0,1008,1700, 1000); // esc ma nastaveny 50% reverse preto offset iba na forward
    }

  

//
//if (speed > 1700){
//  fanOn();}
//else{
//  fanOff();}
  

  esc.writeMicroseconds(speed);
  }
void throttle(int x){
      int speed;
     speed = map(x,0,1022, 1548, 1900);
     if (speed < 1550){
      speed = 1500;}

if (speed > 1700){
  fanOn();}
else{
  fanOff();}
  
  esc.writeMicroseconds(speed);
  }
void reverseThrottle(int x){
      int speed;
     speed = map(x,0,1022, 1000, 1900);
     if (speed < 1550 and speed > 1450){
      speed = 1500;}

if (speed > 1700){
  fanOn();}
else{
  fanOff();}
  
  esc.writeMicroseconds(speed);
  }
void initFan(){
  
pinMode(fanPin, OUTPUT);
digitalWrite(fanPin,HIGH);
}
