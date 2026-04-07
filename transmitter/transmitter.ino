#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <string.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

const int joyX = A0;
const int joyY = A1;
const int joySW = 2;

const int pot = A2;


struct joyData
{
    int x;
    int y;
    bool sw;
    int pot;
    char key;
    
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
joyData data;
joyData last_data;

const int sens = 3;

//keypad

const byte ROWS = 3;
const byte COLS = 2;

char keys[ROWS][COLS] = {
  {  'A' ,'B'},
  { 'C', 'D' },
  {'E', 'F'}
};

// Pin mapping
byte rowPins[ROWS] = {4, 5, 9 };           // R1, R2, R3, R4 as outputs
byte colPins[COLS] = {3,6};        // C1, C2, C3, C4 as inputs with pullup

char scanKeypad() {
  // Return 0 if no key, otherwise the character
  for (byte r = 0; r < ROWS; r++) {
    // Drive current row LOW, others stay HIGH
    digitalWrite(rowPins[r], LOW);
    delayMicroseconds(5);  // let signals settle

    for (byte c = 0; c < COLS; c++) {
      if (digitalRead(colPins[c]) == LOW) {  // active low (because of pull‑ups)
        // Simple debouncing
        delay(20);
        if (digitalRead(colPins[c]) == LOW) {
          // Wait for key release (optional, for single print per press)
          while (digitalRead(colPins[c]) == LOW) {
            delay(5);
          }
          digitalWrite(rowPins[r], HIGH);  // restore row
          return keys[r][c];
        }
      }
    }

    // Restore row to HIGH before moving on
    digitalWrite(rowPins[r], HIGH);
  }

  return 0;  // no key
}

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  Serial.begin(9600);

  pinMode(joySW, INPUT_PULLUP);

  //keypad
  for (byte r = 0; r < ROWS; r++) {
  pinMode(rowPins[r], OUTPUT);
  digitalWrite(rowPins[r], HIGH);
  }

// Columns as inputs with internal pull‑ups
for (byte c = 0; c < COLS; c++) {
  pinMode(colPins[c], INPUT_PULLUP);
  }
}


void loop() {
  
data.x = map(analogRead(joyX),0,1023, 1023,0);
data.y = map(analogRead(joyY),0,1023,1023,0);
data.sw = (digitalRead(joySW) == LOW ? true : false);

data.pot = map(analogRead(pot),0,707,0,1023);
char k = scanKeypad();
if (k != 0) {
  data.key = k;
  }


if (abs(data.x - last_data.x) <= sens) data.x = last_data.x;
if (abs(data.y - last_data.y) <= sens) data.y = last_data.y;
if (abs(data.pot - last_data.pot) <= sens) data.pot = last_data.pot;


  if (memcmp(&data, &last_data, sizeof(joyData)) != 0) {

    
packet.header.controllerID = 1;
packet.header.dataLen = sizeof(joyData);

  memset(packet.payload, 0, sizeof(packet.payload));
  memcpy(packet.payload, &data, sizeof(data));

radio.write(&packet, sizeof(packet));

    

//    Serial.println(data.key );
//    Serial.println(data.y);
//    Serial.println(data.x);
//    Serial.println(data.pot);
//    Serial.println(data.sw);
    
    data.key = 0;
    last_data = data;
  }



//Serial.print("x");


delay(10);

}
