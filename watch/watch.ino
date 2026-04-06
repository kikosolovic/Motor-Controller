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
unsigned long lastSend = 0;

void setup() {
  Serial.begin(9600);

  if (!radio.begin()) {
    Serial.println("nRF24 not responding");
    while (1) {}
  }

  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setRetries(5, 15);
  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.println("TX started");
}

void loop() {
  if (millis() - lastSend >= 1000) {
    lastSend = millis();

    data.counter++;
    data.value = (digitalRead(2)==LOW ? 0 : 1);

    bool ok = radio.write(&data, sizeof(data));

    Serial.print("Sent counter=");
    Serial.print(data.counter);
    Serial.print(" value=");
    Serial.print(data.value);
    Serial.print(" result=");
    Serial.println(ok ? "OK" : "FAIL");
  }
}
