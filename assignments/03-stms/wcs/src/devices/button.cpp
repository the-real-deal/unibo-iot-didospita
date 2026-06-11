#include "button.hpp"

#include "Arduino.h"

PushButton::PushButton(uint8_t pin, EventFamily family, EventManager *eventManager)
    : EventObserver(family),
      EventSource(family, eventManager),
      pin(pin,
          InterruptMode::Change,
          BTN_DEBOUNCE_MILLIS,
          InterruptCallback(this, PushButton::interruptHandler)),
      onPressCallback(), onReleaseCallback() {}

void PushButton::interruptHandler(void *ctx, InterruptPinState pinState)
{
    PushButton *button = static_cast<PushButton *>(ctx);
    ButtonEventType eventType;
    switch (pinState)
    {
    case InterruptPinState::Rising:
        eventType = ButtonEventType::Press;
        break;
    case InterruptPinState::Falling:
        eventType = ButtonEventType::Release;
        break;
    }
    button->generateEvent({.eventType = eventType,
                           .pin = button->getPin()});
}

void PushButton::callCallback(ButtonCallback callback)
{
    if (callback != nullptr)
    {
        callback(this);
    }
}

void PushButton::onEvent(EventSignal *event)
{
    ButtonEvent buttonEvent = static_cast<Event<ButtonEvent> *>(event)->getData();
    if (this->getPin() != buttonEvent.pin)
    {
        return;
    }

    switch (buttonEvent.eventType)
    {
    case ButtonEventType::Press:
        this->onPressCallback(this);
        break;
    case ButtonEventType::Release:
        this->onReleaseCallback(this);
        break;
    }
}

void PushButton::setup()
{
    this->pin.setup();
    this->eventManager->registerObserver(this);
}

uint8_t PushButton::getPin() { return this->pin.getPin(); }

void PushButton::onPress(ButtonCallback callback) { this->onPressCallback = callback; }

void PushButton::onRelease(ButtonCallback callback) { this->onReleaseCallback = callback; }
