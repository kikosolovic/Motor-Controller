#ifndef GPS_H
#define GPS_H

struct gps{
  float la;
  float lo;};
void getGps();
void setGpsTarget();
char gpsTest();
gps gpsTarget;
#endif
