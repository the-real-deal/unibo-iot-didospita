#include "button.hpp"

#include "Arduino.h"

PushButton::PushButton(uint8_t pin)
    : pin(pin,
          InterruptMode::Change,
          BTN_DEBOUNCE_MILLIS,
          InterruptCallback(this, PushButton::interruptHandler)),
      onPressCallback(), onReleaseCallback() {}

void PushButton::interruptHandler(void *ctx, InterruptPinState pinState)
{
    PushButton *button = static_cast<PushButton *>(ctx);
    switch (pinState)
    {
    case InterruptPinState::Rising:
        button->callCallback(button->onPressCallback);
        break;
    case InterruptPinState::Falling:
        button->callCallback(button->onReleaseCallback);
        break;
    }
}

void PushButton::callCallback(ButtonCallback callback)
{
    if (callback != nullptr)
    {
        callback(this);
    }
}

void PushButton::setup()
{
    this->pin.setup();
}

uint8_t PushButton::getPin() { return this->pin.getPin(); }

void PushButton::onPress(ButtonCallback callback) { this->onPressCallback = callback; }

void PushButton::onRelease(ButtonCallback callback) { this->onReleaseCallback = callback; }
