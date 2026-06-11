#pragma once

#include <stdint.h>

#include "core/setup.hpp"
#include "core/interrupts.hpp"

#ifndef BTN_DEBOUNCE_MILLIS
#define BTN_DEBOUNCE_MILLIS 50
#endif

enum class ButtonEvents
{
    Press,
    Release,
};

class PushButton;

using ButtonCallback = void (*)(PushButton*);

class PushButton : public Setup
{
private:
    InterruptPin pin;
    ButtonCallback onPressCallback;
    ButtonCallback onReleaseCallback;

    static void interruptHandler(void *ctx, InterruptPinState state);
    void callCallback(ButtonCallback callback);

public:
    PushButton(uint8_t pin);
    virtual void setup() override;
    uint8_t getPin();
    void onPress(ButtonCallback callback);
    void onRelease(ButtonCallback callback);
};
