#include <Wire.h>
#include "compass.h"
#include "servo.h"


// QMC5883P的I2C地址
const int QMC5883P_ADDR = 0x2C;

// 控制寄存器地址
const int MODE_REG = 0x0A;
const int CONFIG_REG = 0x0B;

// 数据输出寄存器地址
const int X_LSB_REG = 0x01;
const int X_MSB_REG = 0x02;
const int Y_LSB_REG = 0x03;
const int Y_MSB_REG = 0x04;
const int Z_LSB_REG = 0x05;
const int Z_MSB_REG = 0x06;

// 状态寄存器地址
const int STATUS_REG = 0x09;

// 初始化QMC5883P
void initQMC5883P() {
  Wire.begin();
  // 设置为连续测量模式，数据输出速率为200Hz，量程为±8G
  Wire.beginTransmission(QMC5883P_ADDR);
  Wire.write(MODE_REG);
  Wire.write(0xCF); // 0xCF: 连续模式, 200Hz数据输出速率
  Wire.endTransmission();

  Wire.beginTransmission(QMC5883P_ADDR);
  Wire.write(CONFIG_REG);
  Wire.write(0x08); // 0x08: 设置Set/Reset模式为开启，量程为±8G
  Wire.endTransmission();
}

// 读取QMC5883P的原始数据
void readQMC5883PData(int16_t& x, int16_t& y, int16_t& z) {
  Wire.beginTransmission(QMC5883P_ADDR);
  Wire.write(X_LSB_REG);
  Wire.endTransmission(false);
  Wire.requestFrom(QMC5883P_ADDR, 6);

  if (Wire.available() == 6) {
    byte x_lsb = Wire.read();
    byte x_msb = Wire.read();
    byte y_lsb = Wire.read();
    byte y_msb = Wire.read();
    byte z_lsb = Wire.read();
    byte z_msb = Wire.read();

    x = (x_msb << 8) | x_lsb;
    y = (y_msb << 8) | y_lsb;
    z = (z_msb << 8) | z_lsb;
  }
}

// 计算方位角 (Azimuth/Heading)
float calculateAzimuth(int16_t x, int16_t y) {
  x = (x - x_off)*x_scale;
  y = (y - y_off)*y_scale;
  float azimuth = atan2(y, x) * 180.0 / PI;
  
  // 转换为0-360度
  if (azimuth < 0) {
    azimuth += 360.0;
  }
  
  return azimuth;
}

// In your compass.h or main code
#define FILTER_SIZE 5
int xBuffer[FILTER_SIZE], yBuffer[FILTER_SIZE];
int bufIndex = 0;

float smoothAzimuth(int16_t x, int16_t y) {
  xBuffer[bufIndex] = x; yBuffer[bufIndex] = y;
  bufIndex = (bufIndex + 1) % FILTER_SIZE;
  
  long xAvg = 0, yAvg = 0;
  for (int i = 0; i < FILTER_SIZE; i++) {
    xAvg += xBuffer[i]; yAvg += yBuffer[i];
  }
  float azimuth = atan2(yAvg/FILTER_SIZE, xAvg/FILTER_SIZE) * 180/PI;

    // 转换为0-360度
  if (azimuth < 0) {
    azimuth += 360.0;
  }
  
  return azimuth;
}


void calibrateCompass(){
    int16_t x_min=32767, x_max=-32768, y_min=32767, y_max=-32768, z_min=32767, z_max=-32768;
  unsigned long start = millis();

  while (millis() - start < 30000) {  // 30 sec
    int16_t x,y,z;
    readQMC5883PData(x,y,z);
    
    x_min = min(x_min, x); x_max = max(x_max, x);
    y_min = min(y_min, y); y_max = max(y_max, y);
    z_min = min(z_min, z); z_max = max(z_max, z);
    wdt_reset();
    delay(1);
  }

  x_off = (x_min + x_max)/2;
  y_off = (y_min + y_max)/2;
  
  x_scale = (float)(x_max - x_min) / (y_max - y_min);  // Normalize to Y range
  y_scale = 1.0;
  
  Serial.println("=== CALIBRATION DONE ===");
  Serial.print("x_off="); Serial.println(x_off);
  Serial.print("y_off="); Serial.println(y_off);
  Serial.print("x_scale="); Serial.println(x_scale);
  center();
  }

// 获取方向文字 (N, NE, E, etc.)
String getDirection(float azimuth) {
  if (azimuth >= 337.5 || azimuth < 22.5) return "N";
  else if (azimuth < 67.5) return "NE";
  else if (azimuth < 112.5) return "E";
  else if (azimuth < 157.5) return "SE";
  else if (azimuth < 202.5) return "S";
  else if (azimuth < 247.5) return "SW";
  else if (azimuth < 292.5) return "W";
  else return "NW";
}
