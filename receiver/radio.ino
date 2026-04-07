#include <nRF24L01.h>
#include <RF24.h>
#include "radio.h"

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";




void initRadio(){
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

bool getRadioData(){
    if (radio.available()) {
    radio.read(&packet, sizeof(packet));
    return true;
  }
  return false;}
