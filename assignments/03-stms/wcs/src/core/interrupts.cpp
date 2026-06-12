#include "interrupts.hpp"

// define before EnableInterrupt.h to provide arduinoInterruptedPin
// https://github.com/GreyGnome/EnableInterrupt?tab=readme-ov-file#determine-the-pin-that-was-interrupted
#ifndef EI_ARDUINO_INTERRUPTED_PIN
#define EI_ARDUINO_INTERRUPTED_PIN
#endif

#include <EnableInterrupt.h>

uint8_t rawInterruptMode(InterruptMode mode)
{
    switch (mode)
    {
    case InterruptMode::Rising:
        return RISING;
    case InterruptMode::Falling:
        return FALLING;
    case InterruptMode::Change:
        return CHANGE;
    case InterruptMode::High:
        return HIGH;
    case InterruptMode::Low:
        return LOW;
    }
    // unreachable
    return 0;
}

InterruptCallback::InterruptCallback(void *context, Fn fn)
    : context(context), fn(fn) {}

InterruptCallback::InterruptCallback() : InterruptCallback(nullptr, nullptr) {}

bool InterruptCallback::hasHandler()
{
    return this->fn != nullptr;
}

void InterruptCallback::call(InterruptPinState pinState)
{
    if (this->hasHandler())
    {
        this->fn(this->context, pinState);
    }
}

InterruptPin::InterruptHandle::InterruptHandle(uint8_t pin, uint32_t debounceMillis, InterruptCallback callback)
    : pin(pin), debounceMillis(debounceMillis), callback(callback), lastEventTime() {}

InterruptPin::InterruptHandle::InterruptHandle()
    : InterruptPin::InterruptHandle(0, 0, InterruptCallback()) {}

bool InterruptPin::InterruptHandle::isDebouncing()
{
    return millis() - this->lastEventTime < this->debounceMillis;
}

Array<InterruptPin::InterruptHandle, INTERRUPTS_MAX_PINS> InterruptPin::interrupts;

Pair<InterruptPin::InterruptHandle *, size_t> InterruptPin::findPinInterrupt(uint8_t pin)
{
    return InterruptPin::interrupts.find(
        [pin](InterruptHandle *h)
        { return h->pin == pin && h->callback.hasHandler(); });
}

void InterruptPin::interruptHandler()
{
    Pair<InterruptHandle *, size_t> interrupt = InterruptPin::findPinInterrupt(arduinoInterruptedPin);
    if (interrupt.left == nullptr || interrupt.left->isDebouncing())
    {
        return;
    }

    InterruptPinState pinState = arduinoPinState == 0 ? InterruptPinState::Falling
                                                      : InterruptPinState::Rising;
    interrupt.left->lastEventTime = millis();
    interrupt.left->callback.call(pinState);
}

InterruptPin::InterruptPin(uint8_t pin, InterruptMode mode,
                           uint32_t debounceMillis, InterruptCallback callback)
    : handle(pin, debounceMillis, callback), mode(mode) {}

InterruptPin::~InterruptPin()
{
    Pair<InterruptHandle *, size_t> existingInterrupt = InterruptPin::findPinInterrupt(this->handle.pin);
    if (existingInterrupt.left == nullptr)
    {
        return;
    }
    InterruptPin::interrupts.deleteAt(existingInterrupt.right);
}

void InterruptPin::setup()
{
    Pair<InterruptHandle *, size_t> existingInterrupt = InterruptPin::findPinInterrupt(this->handle.pin);
    if (existingInterrupt.left != nullptr)
    {
        abort();
    }
    InterruptPin::interrupts.pushLast(this->handle);
    enableInterrupt(this->handle.pin, InterruptPin::interruptHandler, rawInterruptMode(this->mode));
}

uint8_t InterruptPin::getPin() { return this->handle.pin; }

InterruptMode InterruptPin::getMode() { return this->mode; }

uint32_t InterruptPin::getDebounceMillis() { return this->handle.debounceMillis; }

uint32_t InterruptPin::getLastEventTime() { return this->handle.lastEventTime; }
