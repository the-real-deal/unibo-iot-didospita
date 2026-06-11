#include "button.hpp"

#include "Arduino.h"

PushButton::PushButton(uint8_t pin, EventFamily family, EventsManager *eventManager)
    : EventSource(family, eventManager),
      pin(pin,
          InterruptMode::Change,
          BTN_DEBOUNCE_MILLIS,
          InterruptCallback(this, PushButton::interruptHandler)) {}

void PushButton::interruptHandler(void *ctx, InterruptPinState pinState)
{
    PushButton *button = static_cast<PushButton *>(ctx);
    ButtonEvent event;
    switch (pinState)
    {
    case InterruptPinState::Rising:
        event = ButtonEvent::Press;
        break;
    case InterruptPinState::Falling:
        event = ButtonEvent::Release;
        break;
    }
    button->generateEvent(event);
}

void PushButton::setup()
{
    this->pin.setup();
}
