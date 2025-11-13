#include "button.hpp"
#include "../config.hpp"
#include "lib/led.hpp"
#include "lib/utils.hpp"

bool checkDebounce(const uint64_t interval_ms = DEFAULT_DEBOUNCE_MS) {
  static uint64_t prevts = 0;
  const uint64_t ts = millis();
  if ((ts - prevts) > interval_ms) {
    prevts = ts;
    return false;
  }
  return true;
}

void buttonPressed(const uint8_t pin) {
  if (checkDebounce()) {
    return;
  }

  const size_t index = indexOf<uint8_t>(BUTTON_PINS, SEQUENCE_LENGTH, pin);
  if (index == -1ul) {
    Serial.println("Unknown button " + String(pin) + " pressed!");
    return;
  }

  Serial.println("Pressed button at pin " + String(pin));
  turnOffAllLeds();
  turnOnLed(index);
}