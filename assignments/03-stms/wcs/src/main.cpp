#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  delay(2000); 

  Serial.println("SERIAL_SYNC");
  Serial.println("setup() finished");
}

void loop() {
  Serial.println("LOG:loop() is running");
  delay(1000);
}