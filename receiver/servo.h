#ifndef SERVO_H
#define SERVO_H


void turn(float x);
void initServo();
void calibrate();
void setAngle(float x);
void continuousTurn(int x);
void unrestrictedTurn(float x);
int16_t getCX();
float current_angle;
int whole_turn_angle = 114;
int degPerTurn = 5;
void center();

bool CLEnabled = false;
 unsigned long lastServoTurn = 0;
int servoInterval = 5;
int compassInterval = 100;
void angleTest(int x);
unsigned long nowSer;
#endif
