#include "../lib/led.hpp"
#include "../config.hpp"

#define LED_FADE_LBOUND 0
#define LED_FADE_UBOUND 255

const int32_t FADE_DELTATIME = CONTROL_LED_FADE_AMOUNT / LOOP_DELAY_MS;

void turnOnAllGameLeds() {
  for (size_t i = 0; i < SEQUENCE_LENGTH; i++) {
    turnOnLed(GAME_LEDS_PINS[i]);
  }
}

void turnOffAllGameLeds() {
  for (size_t i = 0; i < SEQUENCE_LENGTH; i++) {
    turnOffLed(GAME_LEDS_PINS[i]);
  }
}

void fadeLed(const uint8_t pin, int32_t *const currentFade) {
  static int32_t fadeAmount = FADE_DELTATIME;

  *currentFade += fadeAmount;
  if (*currentFade <= LED_FADE_LBOUND) {
    fadeAmount = FADE_DELTATIME;
    *currentFade = LED_FADE_LBOUND;
  } else if (*currentFade >= LED_FADE_UBOUND) {
    fadeAmount = -FADE_DELTATIME;
    *currentFade = LED_FADE_UBOUND;
  }

  analogWrite(pin, *currentFade);
}
