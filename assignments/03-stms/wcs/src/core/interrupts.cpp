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

Interrupt::Interrupt(uint8_t pin, void *handlerContext, InterruptHandler handler)
    : pin(pin), handlerContext(handlerContext), handler(handler) {}

Interrupt::Interrupt() : pin(), handlerContext(), handler() {}

uint8_t Interrupt::getPin() { return this->pin; }

InterruptHandler Interrupt::getHandler() { return this->handler; }

void *Interrupt::getHandlerContext() { return this->handlerContext; }

bool Interrupt::hasHandler() { return this->handler != nullptr; }

Array<Interrupt, INTERRUPTS_MAX_PINS> InterruptPin::interrupts;

Pair<Interrupt *, size_t> InterruptPin::findPinInterrupt(uint8_t pin)
{
    return InterruptPin::interrupts.find(
        [pin](Interrupt *i)
        { return i->getPin() == pin && i->hasHandler(); });
}

void InterruptPin::interruptHandler()
{
    Pair<Interrupt *, size_t> interrupt = InterruptPin::findPinInterrupt(arduinoInterruptedPin);
    if (interrupt.left == nullptr)
    {
        return;
    }
    InterruptPinState pinState = arduinoPinState == 0 ? InterruptPinState::Falling
                                                      : InterruptPinState::Rising;
    interrupt.left->getHandler()(interrupt.left->getHandlerContext(), pinState);
}

InterruptPin::InterruptPin(uint8_t pin, InterruptMode mode, void *context, InterruptHandler handler)
    : pin(pin), mode(mode), context(context), handler(handler) {}

InterruptPin::~InterruptPin()
{
    Pair<Interrupt *, size_t> existingInterrupt = InterruptPin::findPinInterrupt(this->pin);
    if (existingInterrupt.left == nullptr)
    {
        return;
    }
    InterruptPin::interrupts.deleteAt(existingInterrupt.right);
}

void InterruptPin::setup()
{
    Pair<Interrupt *, size_t> existingInterrupt = InterruptPin::findPinInterrupt(this->pin);
    if (existingInterrupt.left != nullptr)
    {
        exit(-1);
    }
    InterruptPin::interrupts.pushLast(Interrupt(this->pin, this->context, this->handler));
    enableInterrupt(this->pin, InterruptPin::interruptHandler, rawInterruptMode(this->mode));
}

uint8_t InterruptPin::getPin() { return this->pin; }

InterruptMode InterruptPin::getMode() { return this->mode; }
