#include <Arduino.h>
#define LED_PIN 9

int brightness;
int fadeAmount;
int currIntensity;

void setup() {
  currIntensity = 0;
  fadeAmount = 5;
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  analogWrite(LED_PIN, currIntensity);
  currIntensity += fadeAmount;
  if (currIntensity == 0 || currIntensity == 255) {
    fadeAmount = -fadeAmount;
  }
  delay(15);
}
