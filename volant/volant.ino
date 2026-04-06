#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "NODE1";

struct Payload {
  unsigned long counter;
  int value;
};

Payload data;

void setup() {
  Serial.begin(9600);

  if (!radio.begin()) {
    Serial.println("nRF24 not responding");
    while (1) {}
  }

  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0, address);
  radio.startListening();

  Serial.println("RX started");
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(data));

    Serial.print("Received counter=");
    Serial.print(data.counter);
    Serial.print(" value=");
    Serial.println(data.value);
  }
}
