#ifndef ESC_H
#define ESC_H


void initEsc();
void throttle(int x);
void throttleJoy(int x);
void initFan();
void toggleFan();
bool offfan = true;
bool motorOn = false;

#endif
