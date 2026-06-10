#include "button.hpp"

#include "Arduino.h"

PushButton::PushButton(uint8_t pin)
    : pin(pin,
          InterruptMode::Change,
          BTN_DEBOUNCE_MILLIS,
          InterruptCallback(this, PushButton::interruptHandler)) {}

void PushButton::interruptHandler(void *ctx, InterruptPinState state)
{
    PushButton *button = static_cast<PushButton *>(ctx);
    Serial.print(F("Button on pin "));
    Serial.print(button->pin.getPin());
    Serial.print(F(" changed to "));
    Serial.println(state == InterruptPinState::Falling ? F("Falling") : F("Rising"));
    Serial.flush();
}

void PushButton::setup()
{
    this->pin.setup();
}