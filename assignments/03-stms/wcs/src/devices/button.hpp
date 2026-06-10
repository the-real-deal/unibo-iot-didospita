#pragma once

#include <stdint.h>

#include "core/setup.hpp"
#include "core/interrupts.hpp"

class PushButton : public Setup
{
private:
    InterruptPin pin;
    static void interruptHandler(void* ctx, InterruptPinState state);
public:
    PushButton(uint8_t pin,
        InterruptHandler onPress = nullptr,
        InterruptHandler onRelease = nullptr);
    virtual void setup() override;
};
