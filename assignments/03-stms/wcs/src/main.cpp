#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  delay(2000); 

  Serial.println("setup() finished");
}

void loop() {
  Serial.println("loop() is running");
  delay(1000);
}