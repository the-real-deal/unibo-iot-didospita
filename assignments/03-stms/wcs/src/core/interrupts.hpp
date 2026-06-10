#pragma once

#include <stdint.h>

#include "setup.hpp"
#include "std/collections.hpp"
#include "std/pair.hpp"

#ifndef INTERRUPTS_MAX_PINS
#define INTERRUPTS_MAX_PINS 5
#endif

// EnableInterrupt must be compiled into exactly one object file, or multiple definitions
// error will occur. So the library is imported in the .cpp file.

enum class InterruptMode
{
    Rising,
    Falling,
    Change,
    High,
    Low,
};

enum class InterruptPinState
{
    Falling,
    Rising,
};

class InterruptCallback
{
public:
    using Fn = void (*)(void *, InterruptPinState);

private:
    void *context;
    Fn fn;

public:
    InterruptCallback(void *context, Fn fn);
    InterruptCallback();
    bool hasHandler();
    void call(InterruptPinState pinState);
};

class InterruptPin : public Setup
{
private:
    class InterruptHandle
    {
    public:
        uint8_t pin;
        uint32_t debounceMillis;
        InterruptCallback callback;
        uint32_t lastEventTime;

        InterruptHandle(uint8_t pin, uint32_t debounceMillis, InterruptCallback callback);
        InterruptHandle();

        bool isDebouncing();
    };

    static Array<InterruptHandle, INTERRUPTS_MAX_PINS> interrupts;

    InterruptHandle handle;
    InterruptMode mode;

    static Pair<InterruptHandle *, size_t> findPinInterrupt(uint8_t pin);
    static void interruptHandler();

public:
    InterruptPin(uint8_t pin, InterruptMode mode,
                 uint32_t debounceMillis, InterruptCallback callback);
    ~InterruptPin();

    virtual void setup() override;
    uint8_t getPin();
    InterruptMode getMode();
    uint32_t getDebounceMillis();
    uint32_t getLastEventTime();
};
