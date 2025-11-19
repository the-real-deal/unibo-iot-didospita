#include "button.hpp"
#include "../config.hpp"
#include "../lib/utils.hpp"
#include "led.hpp"
#include "lib/led.hpp"
#include <assert.h>

bool checkDebounce(const uint64_t interval_ms, const size_t index) {
  static uint64_t prevts[SEQUENCE_LENGTH] = {0};
  const uint64_t ts = millis();
  if ((ts - prevts[index]) > interval_ms) {
    prevts[index] = ts;
    return false;
  }
  return true;
}

void buttonPressed(const uint8_t pin, Game *const game, const size_t index) {
  if (checkDebounce(DEFAULT_DEBOUNCE_MS, index)) {
    return;
  }
  if (pin == BUTTON_PINS[WAKE_BTN_INDEX]) {
    switch (game->state) {
    case SLEEP:
      disableSleep();
      /*
        cannot change state directly after deepSleep() in game step
        because you cannot init deep sleeping again inside an interrupt.
      */
      game->state = INITIAL;
      break;
    case MENU:
      game->state = INIT_GAME;
      break;
    default:
      break;
    }
  }

  if (game->state == PLAYING) {
    assert(index != -1ul);
    turnOffAllGameLeds();
    turnOnLed(GAME_LEDS_PINS[index]);
    advanceSequence(&game->sequence, index + 1);
  }
}
