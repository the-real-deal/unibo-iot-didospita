#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(2000); 

  Serial.print("setup() is running on core ");
  Serial.println(xPortGetCoreID());
}

void loop() {
  Serial.print("loop() is running on core ");
  Serial.println(xPortGetCoreID());
  delay(1000);
}