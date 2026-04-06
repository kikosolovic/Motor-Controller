
#include <TinyGPS++.h>
//#include <SoftwareSerial.h>
#include "AltSoftSerial.h"
#include <math.h>

float toRadians(float degrees) {
  return degrees * M_PI / 180.0;
}

float calculateBearing(float lat1, float lon1, float lat2, float lon2) {
  float dLon = toRadians(lon2 - lon1);
  lat1 = toRadians(lat1); lat2 = toRadians(lat2);
  
  float y = sin(dLon) * cos(lat2);
  float x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);
  float bearing = atan2(y, x);
  
  bearing = fmod(bearing * 180.0 / M_PI + 360.0, 360.0);
  return bearing;
}




static const int RXPin = 4, TXPin = 5;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;
AltSoftSerial ss(RXPin, TXPin);

void initGps(){
  Serial.begin(9600);
  ss.begin(GPSBaud);

}



void getGps(){
  while (ss.available() > 0){
    char c = ss.read();
    Serial.print(c);           // OPTIONAL: Keep raw NMEA for debug
    gps.encode(c);             // FEED TinyGPS++ ← MISSING!
    
    if (gps.location.isUpdated()) {
      float lat = gps.location.lat();
      float lng = gps.location.lng();
      
      // Calculate bearing to target (e.g. Sønderborg)
      
      float targetLat = 54.921845;  // Your location
      float targetLng = 9.780012;
      float bearing = calculateBearing(lat, lng, targetLat, targetLng);
      
      Serial.print(" Lat: "); Serial.print(lat, 6);
      Serial.print(" Lng: "); Serial.println(lng, 6);
      Serial.print(" Bearing: "); Serial.println(bearing, 1);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
initGps();
}

void loop() {
  getGps();
  // put your main code here, to run repeatedly:

}
