#include "button.hpp"
#include "../config.hpp"
#include "lib/led.hpp"
#include "lib/utils.hpp"
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

void buttonPressed(const uint8_t pin) {
  if (checkDebounce(DEFAULT_DEBOUNCE_MS)) {
    return;
  }

  const size_t index = indexOf<uint8_t>(BUTTON_PINS, SEQUENCE_LENGTH, pin);
  assert(index != -1ul);

  Serial.println("Pressed button at pin " + String(pin));
  turnOffAllLeds();
  turnOnLed(index);
}