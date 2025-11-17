#include "../lib/led.hpp"
#include "../config.hpp"

void turnOnAllGameLeds() {
  for (size_t i = 0; i < SEQUENCE_LENGTH; i++) {
    turnOnLed(GAME_LEDS_PINS[i]);
  }
};

void turnOffAllGameLeds() {
  for (size_t i = 0; i < SEQUENCE_LENGTH; i++) {
    turnOffLed(GAME_LEDS_PINS[i]);
  }
};
