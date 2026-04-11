#include <Servo.h>
#include "compass.h"
#include "compassLock.h"


Servo servi;
int serPin = 2;


void setAngle(float x) {
  x = constrain(x, 705, 2347.5);
//  Serial.println("setting angle at: " + String(x));
  current_angle = x;
}
void unrestrictedTurn (float x){
  servi.writeMicroseconds(x);}
  
void turn(float x) {
  x = constrain(x, 705, 2347.5);
  servi.writeMicroseconds(x);
  setAngle(x);
}



void continuousTurn(int x) {

  nowSer = millis();
if (nowSer - lastServoTurn >= servoInterval){
  //0 - 1023 -- 503 neutral
  if (abs(x - 503) < 3){

    if (CLEnabled){
      if (!locked){
        delay(100);
        setTarget();
        locked = true;
        
        }
    compassLock();
    }
    return;}
   
  int loop_turn = map(x, 0, 1022, (-1) * degPerTurn, degPerTurn);
  setAngle(current_angle + loop_turn);
  servi.write(current_angle);
  locked = false;
//      setTarget();


lastServoTurn = nowSer;

}}
void center(){
  turn(1527.5);
  delay(600);
  target = Azimuth();}

void initServo() {
    turn(1527.5);
  servi.attach(serPin);
}

void angleTest(int x){
  servi.writeMicroseconds(map(x,0,1023,544,2400));}

int16_t getCX(){
    int16_t x, y, z;
  readQMC5883PData(x, y, z);
  Serial.println("z : " + String(z));
  Serial.println("y : " + String(y));
  return x;
  }

void calibrate() {

    turn(70);
  delay(2000);

  int initial_angle = Azimuth();
  turn(110);
  delay(2000);

  int final_angle = Azimuth();


  float whole_turn_angle = initial_angle - final_angle ;
  if (whole_turn_angle > 180) whole_turn_angle -= 360;
  if (whole_turn_angle < -180) whole_turn_angle += 360;
  
  Serial.println("initial = " + String(initial_angle));
  Serial.println("final = " + String(final_angle));
  Serial.println("effective = " + String(whole_turn_angle));
  coef =  40 / abs(whole_turn_angle) ;
  Serial.println(coef);
  

}
