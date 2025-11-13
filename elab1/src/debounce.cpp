#include "debounce.hpp"
#include <Arduino.h>

bool checkDebounce(const uint64_t interval_ms) {
  static uint64_t prevts = 0;
  const uint64_t ts = millis();
  if ((ts - prevts) > interval_ms) {
    prevts = ts;
    return false;
  }
  return true;
}
bool checkDebounce() { return checkDebounce(DEFAULT_DEBOUNCE_MS); }
