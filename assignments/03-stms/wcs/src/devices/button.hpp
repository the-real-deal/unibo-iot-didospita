#pragma once

#include <stdint.h>

#include "core/setup.hpp"
#include "core/interrupts.hpp"

#ifndef BTN_DEBOUNCE_MILLIS
#define BTN_DEBOUNCE_MILLIS 200
#endif

class PushButton : public Setup
{
private:
    InterruptPin pin;
    static void interruptHandler(void* ctx, InterruptPinState state);
public:
    PushButton(uint8_t pin);
    virtual void setup() override;
};
