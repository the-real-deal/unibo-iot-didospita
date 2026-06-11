#pragma once

#include <stdint.h>

#include "core/setup.hpp"
#include "core/events.hpp"
#include "core/interrupts.hpp"

#ifndef BTN_DEBOUNCE_MILLIS
#define BTN_DEBOUNCE_MILLIS 50
#endif

enum class ButtonEventType
{
    Press,
    Release,
};

struct ButtonEvent
{
    ButtonEventType eventType;
    uint8_t pin;
};

class PushButton;

using ButtonCallback = void (*)(PushButton *);

class PushButton : public Setup,
                   public EventObserver,
                   public EventSource<ButtonEvent>
{
private:
    InterruptPin pin;
    ButtonCallback onPressCallback;
    ButtonCallback onReleaseCallback;

    static void interruptHandler(void *ctx, InterruptPinState state);
    void callCallback(ButtonCallback callback);

protected:
    void onEvent(EventSignal *event) override;

public:
    PushButton(uint8_t pin, EventFamily family, EventManager* eventManager);
    virtual void setup() override;
    uint8_t getPin();
    void onPress(ButtonCallback callback);
    void onRelease(ButtonCallback callback);
};
