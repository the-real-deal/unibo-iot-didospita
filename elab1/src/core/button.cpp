#include "button.hpp"
#include "../config.hpp"
#include "led.hpp"
#include "lib/led.hpp"
#include "../lib/utils.hpp"
#include <assert.h>

bool checkDebounce(const uint64_t interval_ms) {
  static uint64_t prevts = 0;
  const uint64_t ts = millis();
  if ((ts - prevts) > interval_ms) {
    prevts = ts;
    return false;
  }
  return true;
}

void buttonPressed(const uint8_t pin, Game *const game) {
  if (checkDebounce(DEFAULT_DEBOUNCE_MS)) {
    return;
  }

  Serial.println("Game state: " + String(game->state));
  Serial.println("Pin: " + String(pin));
  switch (game->state) {
  case SLEEP:
    Serial.println("SLEEP");
    if (pin == BUTTON_PINS[WAKE_BTN_INDEX]) {
      Serial.println("wake btn");
      Serial.flush();
      disableSleep();
      /*
       cannot change state directly after deepSleep() in game step
       because you cannot init deep sleeping again inside an interrupt.
      */
      game->state = INITIAL;
    }
    break;
  default:
    Serial.println("DEFAULT");
    const size_t index = indexOf<uint8_t>(BUTTON_PINS, SEQUENCE_LENGTH, pin);
    assert(index != -1ul);

    Serial.println("Pressed button at pin " + String(pin));
    turnOffAllGameLeds();
    turnOnLed(GAME_LEDS_PINS[index]);
    break;
  }
  Serial.println("New state: " + String(game->state));
  Serial.flush();
}