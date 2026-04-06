#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <string.h>



//keypad

const byte ROWS = 1;
const byte COLS = 1;

char keys[ROWS][COLS] = {
  {  'A' }
};

// Pin mapping
byte rowPins[ROWS] = {5 };           // R1, R2, R3, R4 as outputs
byte colPins[COLS] = {6};        // C1, C2, C3, C4 as inputs with pullup

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

  Serial.begin(9600);

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

char k = scanKeypad();



Serial.println(k);

delay(50);

}
