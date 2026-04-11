#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

const int BTN1_PIN = 3;
const int BTN2_PIN = 4;
const int BTN3_PIN = 2;   // D1

struct watchData
{
  uint8_t btn1 = 0;
  uint8_t btn2 = 0;
  uint8_t btn3 = 0;
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
watchData data;
watchData last_data;
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  if (BTN1_PIN != -1) pinMode(BTN1_PIN, INPUT_PULLUP);
  if (BTN2_PIN != -1) pinMode(BTN2_PIN, INPUT_PULLUP);
  if (BTN3_PIN != -1) pinMode(BTN3_PIN, INPUT_PULLUP);
Serial.println("started");
}


void loop() {
    data.btn3 = readSingleButton(BTN3_PIN);
    data.btn1 = (digitalRead(BTN1_PIN)==LOW ? 1 : 0);
    data.btn2 = (digitalRead(BTN2_PIN)==LOW ? 1 : 0);


  if (memcmp(&data, &last_data, sizeof(watchData)) != 0) {
    packet.header.controllerID = 2;
    packet.header.dataLen = sizeof(watchData);

//    Serial.println("s");
      memset(packet.payload, 0, sizeof(packet.payload));
  memcpy(packet.payload, &data, sizeof(data));
radio.write(&packet, sizeof(packet));

    }
    last_data = data;


  }


char readSingleButton(int buttonPin) {
    if (digitalRead(buttonPin) == LOW) {  
      delay(20);                          
      if (digitalRead(buttonPin) == LOW) {
        while (digitalRead(buttonPin) == LOW) {
          delay(5);                      
        }

        return 1;                       
      }
    }
  return 0;  
  }
