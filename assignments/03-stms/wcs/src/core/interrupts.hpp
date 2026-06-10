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

using InterruptHandler = void (*)(void *, InterruptPinState);

class Interrupt
{
private:
    uint8_t pin;
    void *handlerContext;
    InterruptHandler handler;

public:
    Interrupt(uint8_t pin, void *handlerContext, InterruptHandler handler);
    Interrupt();

    uint8_t getPin();
    InterruptHandler getHandler();
    void *getHandlerContext();
    bool hasHandler();
};

class InterruptPin : public Setup
{
private:
    static Array<Interrupt, INTERRUPTS_MAX_PINS> interrupts;

    uint8_t pin;
    InterruptMode mode;
    void *context;
    InterruptHandler handler;

    static Pair<Interrupt *, size_t> findPinInterrupt(uint8_t pin);
    static void interruptHandler();

public:
    InterruptPin(uint8_t pin, InterruptMode mode, void *context, InterruptHandler handler);
    ~InterruptPin();

    virtual void setup() override;
    uint8_t getPin();
    InterruptMode getMode();
};
