#ifndef QMC5883P_H
#define QMC5883P_H

#include "Arduino.h"
#include <Wire.h>
//
//const int QMC5883P_ADDR = 0x2C;
//
//const int MODE_REG   = 0x0A;
//const int CONFIG_REG = 0x0B;
//
//const int X_LSB_REG = 0x01;
//const int X_MSB_REG = 0x02;
//const int Y_LSB_REG = 0x03;
//const int Y_MSB_REG = 0x04;
//const int Z_LSB_REG = 0x05;
//const int Z_MSB_REG = 0x06;
//
//const int STATUS_REG = 0x09;

// Function declarations
void initQMC5883P();
void readQMC5883PData(int16_t& x, int16_t& y, int16_t& z);
float calculateAzimuth(int16_t x, int16_t y);
String getDirection(float azimuth);
float smoothAzimuth(int16_t x, int16_t y);
void calibrateCompass();

//calibration
int x_off = 250, y_off = -100;  
float x_scale = 1.15, y_scale = 1.0;

#endif
