#ifndef RADIO_H
#define RADIO_H

#include "Arduino.h"

void initRadio();
void getRadioData();

struct joyData
{
    int x = 503;
    int y;
    bool sw;
    int pot = 0;
    char key=0;
};

joyData data;

#endif
