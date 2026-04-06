#include "compassLock.h"
#include "servo.h"
   unsigned long currentTime;
   unsigned long lastTurn = 0;
//   int turn_interval = 20;

void setTarget(){
  target = Azimuth();
//  Serial.println("started compass lock at "+ String(target));
 currentTime = millis(); 
  }


float Azimuth(){
  int16_t x, y, z;
  readQMC5883PData(x, y, z);
  return (calculateAzimuth(x, y));
  
  }
  float SmoothAzimuth(){
  int16_t x, y, z;
  readQMC5883PData(x, y, z);
  return (smoothAzimuth(x, y));
  
  }

  
void compassLock(){
unsigned long now = millis();
if (now - lastTurn >= cInterval){
  
    float azimuth = Azimuth();

  float diff = target - azimuth ;
  if (diff > 180) diff -= 360;
  if (diff < -180) diff += 360;
  
  float servo_change = map((diff),-82,82,-500,500);

setAngle(float(current_angle + servo_change * 0.1 ));
    turn(current_angle);
//    Serial.println("current angle: " + String(current_angle));
//    Serial.println("current azimuth: " + String(azimuth));
//    Serial.println("target " + String(target));
//    delay(100);
//
    lastTurn = now;
  }

}
   
  
