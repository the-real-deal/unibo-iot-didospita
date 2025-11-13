#include "button.hpp"
#include "debounce.hpp"
#include "led.hpp"
#include "utils.hpp"

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