#include "ServoTimer2.h"
ServoTimer2 servi;

void setup() {
  // put your setup code here, to run once:
  servi.attach(2);
}

void loop() {
  servi.write(1527);
  delay(1000);
  servi.write(1000);
  delay(1000);
  servi.write(2000);
  delay(10000);

}
