#ifndef RADIO_H
#define RADIO_H

#include "Arduino.h"

void initRadio();
bool getRadioData();

struct steerData
{
  uint16_t throttle;
  uint16_t steerAngle;

  uint8_t btn1;
  uint8_t btn2;
  uint8_t btn3;
  };

struct watchData
{
  uint8_t btn1;
  uint8_t btn2;
  uint8_t btn3;
  };
  
struct joyData
{
    int x = 503;
    int y;
    bool sw;
    int pot = 0;
    char key=0;
};

struct PacketHeader 
{
  uint8_t controllerID;
  uint8_t dataLen;
  };

struct Packet
{
  PacketHeader header;
  uint8_t payload[24];
  };

Packet packet;
  
#endif
