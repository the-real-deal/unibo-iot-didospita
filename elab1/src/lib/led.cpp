#include "led.hpp"
#include "../config.hpp"
#include <Arduino.h>

void turnOnLed(const size_t index) {
  const uint8_t pin = GAME_LEDS_PINS[index];
  Serial.println("Turning on led at pin " + String(pin));
  digitalWrite(pin, HIGH);
};

void turnOnAllLeds() {
  for (size_t i = 0; i < SEQUENCE_LENGTH; i++) {
    turnOnLed(i);
  }
};

void turnOffLed(const size_t index) {
  const uint8_t pin = GAME_LEDS_PINS[index];
  Serial.println("Turning off led at pin " + String(pin));
  digitalWrite(pin, LOW);
};

void turnOffAllLeds() {
  for (size_t i = 0; i < SEQUENCE_LENGTH; i++) {
    turnOffLed(i);
  }
};
