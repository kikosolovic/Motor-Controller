#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

const int throttlePin = A4;
const int steerPin= A1;

const int btn1 = A2;
const int btn2 = A3;
const int btn3 = 0;
bool transmit = true;
struct steerData
{
  uint16_t throttle;
  uint16_t steerAngle;

  uint8_t btn1;
  uint8_t btn2;
  uint8_t btn3;
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

steerData data;
steerData last_data;
Packet packet;
//const uint8_t sens = 15;


void setup() {
  Serial.begin(9600);


radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  if (btn1 != -1) pinMode(btn1, INPUT_PULLUP);
  if (btn2 != -1) pinMode(btn2, INPUT_PULLUP);
  if (btn3 != -1) pinMode(btn3, INPUT_PULLUP);

}

char readSingleButton(int buttonPin) {
  if (digitalRead(buttonPin) == LOW) {  // active-low with pull-up
    delay(20);                          // debounce
    if (digitalRead(buttonPin) == LOW) {
      while (digitalRead(buttonPin) == LOW) {
        delay(5);                       // wait for release
      }
      return 1;                       // or whatever character
    }
  }
  return 0;  // no press
}
void sendData(){
      packet.header.controllerID = 3;
    packet.header.dataLen = sizeof(steerData);
    
      memset(packet.payload, 0, sizeof(packet.payload));
      memcpy(packet.payload, &data, sizeof(data));
    
    radio.write(&packet, sizeof(packet));
       
  }

void loop() {
data.btn1 = readSingleButton(btn1);
data.btn2 = readSingleButton(btn2);
data.btn3 = readSingleButton(btn3);
data.throttle = constrain(map(analogRead(throttlePin),400, 800, 1023,0),0,1023);
data.steerAngle = constrain(map(analogRead(steerPin),0,1023,1023,0),0,1023);



if (data.btn3 == 1){
  transmit = !transmit;

    if (!transmit){
      sendData();} 
    else{
      data.btn3 =2;}

}



//if (abs(data.throttle - last_data.throttle) <= sens) data.throttle = last_data.throttle;
//if (abs(data.steerAngle - last_data.steerAngle) <= sens) data.steerAngle = last_data.steerAngle;

if (transmit){

  if (memcmp(&data, &last_data, sizeof(steerData)) != 0)  {

    
    sendData();
    

//    Serial.println(data.btn1 );
//    Serial.println(data.btn2);
//    Serial.println(data.btn3);
//    Serial.println(data.throttle);
//    Serial.println(data.steerAngle);
    
    last_data = data;
  }}


}
