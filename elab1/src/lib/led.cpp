#include "led.hpp"
#include "../config.hpp"
#include <Arduino.h>

#define LED_FADE_LOWER_BOUND 0
#define LED_FADE_UPPER_BOUND 255

void turnOnLed(const uint8_t pin) { digitalWrite(pin, HIGH); };

void turnOffLed(const uint8_t pin) { digitalWrite(pin, LOW); };

void fadeLed(const uint8_t pin, int32_t *const currentFade) {
  static int32_t fadeAmount = CONTROL_LED_FADE_AMOUNT;
  *currentFade += fadeAmount;
  if (*currentFade <= LED_FADE_LOWER_BOUND) {
    fadeAmount = CONTROL_LED_FADE_AMOUNT;
    *currentFade = LED_FADE_LOWER_BOUND;
  } else if (*currentFade >= LED_FADE_UPPER_BOUND) {
    fadeAmount = -CONTROL_LED_FADE_AMOUNT;
    *currentFade = LED_FADE_UPPER_BOUND;
  }
  analogWrite(pin, *currentFade);
}
