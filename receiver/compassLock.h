#ifndef COMPASSLOCK_H
#define COMPASSLOCK_H

void compassLock();
void setTarget();

float target;
unsigned long previousMillis = 0;
int cInterval =20;
float SmoothAzimuth();
float Azimuth();
float coef = 1.0;

#endif
