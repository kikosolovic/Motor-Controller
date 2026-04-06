#include <Servo.h>;

Servo esc;
int potP = A4;
int potV;
int dO;

void setup(){
  Serial.begin(9600);
  delay(3000);
  esc.attach(4);

//    esc.writeMicroseconds(2000);  // Full throttle (20ms)
//  delay(50);                    // Shortest reliable pulse (~2.5 cycles)
//  
//  esc.writeMicroseconds(1000);  // Full brake (20ms)  
//  delay(50);                    // Shortest reliable pulse
//  
 esc.writeMicroseconds(1500);  // Neutral/stop - stays here
  delay(1000);                   // Settle
//  
  }

  void loop(){
    potV = analogRead(potP);
    
    dO = map(potV,0,663,1000, 2000);
    Serial.println(dO);
    esc.writeMicroseconds(dO);
    delay(300);
    
    
    
    
    }
