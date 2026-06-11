#pragma once

#include <stdint.h>

#include "core/setup.hpp"
#include "core/events.hpp"
#include "core/interrupts.hpp"

#ifndef BTN_DEBOUNCE_MILLIS
#define BTN_DEBOUNCE_MILLIS 50
#endif

enum class ButtonEvent
{
    Press,
    Release,
};

class PushButton;

class PushButton : public Setup,
                   public EventSource<ButtonEvent>
{
private:
    InterruptPin pin;

    static void interruptHandler(void *ctx, InterruptPinState state);

public:
    PushButton(uint8_t pin, EventFamily family, EventsManager* eventManager);
    virtual void setup() override;
};
