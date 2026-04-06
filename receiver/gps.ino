#include "gps.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
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
SoftwareSerial ss(RXPin, TXPin);

void initGps(){
  ss.begin(GPSBaud);

}

char gpsTest(){
  while (ss.available() > 0){
    return ss.read();}
  }

void getGps(){

        float targetLat = 54.921109550268326;  // Your location
      float targetLng =  9.780300616293825;
        float bearing = calculateBearing(54.921195223381055, 9.78037336267004, targetLat, targetLng);
              Serial.print(" Bearing: "); Serial.println(bearing, 1);
      
  while (ss.available() > 0){
    char c = ss.read();
    Serial.print(c);           // OPTIONAL: Keep raw NMEA for debug
    gps.encode(c);             // FEED TinyGPS++ ← MISSING!
    
    if (gps.location.isUpdated()) {
      float lat = gps.location.lat();
      float lng = gps.location.lng();
      
      // Calculate bearing to target (e.g. Sønderborg)
      
      float targetLat = 54.922697;  // Your location
      float targetLng =  9.783709;
      float bearing = calculateBearing(54.921279, 9.780233, targetLat, targetLng);
      
      Serial.print(" Lat: "); Serial.print(lat, 6);
      Serial.print(" Lng: "); Serial.println(lng, 6);
      Serial.print(" Bearing: "); Serial.println(bearing, 1);
    }
  }
}
