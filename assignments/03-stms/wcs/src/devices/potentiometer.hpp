#pragma once

#include <stdint.h>

#include "core/setup.hpp"
#include "core/pins.hpp"
#include "core/events.hpp"

#ifndef POT_EVENT_THRESHOLD
#define POT_EVENT_THRESHOLD 0.03
#endif

struct PotentiometerEvent
{
    double value;
};

class Potentiometer : public Setup, public SyncEventSource<PotentiometerEvent>
{
private:
    AnalogInputPin pin;
    double value;

    double readPin();

public:
    Potentiometer(uint8_t pin, EventFamily family, EventsManager *eventManager);
    void setup() override;
    void checkEvents() override;
    double getValue();
};
