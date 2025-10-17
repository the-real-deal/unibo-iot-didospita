#include <Arduino.h>

#define pinButton 2
#define pinLed 6
bool on;

void setup() {
  pinMode(pinLed, OUTPUT);
  pinMode(pinButton, INPUT);
  Serial.begin(9600);
}

void loop() {
  int buttonState = digitalRead(pinButton);
  if (buttonState == HIGH) {
    digitalWrite(pinLed, HIGH);
    Serial.println("ON");
  } else {
    digitalWrite(pinLed, LOW);
    Serial.println("OFF");
  }
}
