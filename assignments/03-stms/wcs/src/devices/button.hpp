#pragma once

#include <stdint.h>

#include "kernel/events.hpp"
#include "kernel/interrupts.hpp"

#ifndef BTN_DEBOUNCE_MILLIS
#define BTN_DEBOUNCE_MILLIS 150
#endif

enum class ButtonEvent
{
    Press,
    Release,
};

class PushButton;

class PushButton : public EventSource<ButtonEvent>
{
private:
    InterruptPin pin;

    static void interruptHandler(void *ctx, InterruptPinState state);

public:
    PushButton(uint8_t pin, EventFamily family);
    void begin(EventsManager *eventsManager) override;
};
