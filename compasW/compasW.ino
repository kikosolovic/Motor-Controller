#include <Wire.h>

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
  float azimuth = atan2(y, x) * 180.0 / PI;
  
  // 转换为0-360度
  if (azimuth < 0) {
    azimuth += 360.0;
  }
  
  return azimuth;
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

void setup() {
  Serial.begin(9600);
  initQMC5883P();
  Serial.println("QMC5883P Compass Ready - Rotate flat on table!");
}

void loop() {
  int16_t x, y, z;
  readQMC5883PData(x, y, z);

  float azimuth = calculateAzimuth(x, y);
  
  // Tønder磁偏角修正 (~3°E)
  azimuth += 3.0;
  if (azimuth >= 360.0) azimuth -= 360.0;
  
  String direction = getDirection(azimuth);

  Serial.print("X: ");
  Serial.print(x);
  Serial.print("\tY: ");
  Serial.print(y);
  Serial.print("\tZ: ");
  Serial.print(z);
  Serial.print("\tAzimuth: ");
  Serial.print(azimuth, 1);
  Serial.print("°\t");
  Serial.print(direction);
  Serial.println();

  delay(200);
}
