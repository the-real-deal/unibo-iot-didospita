#pragma once

#include <stdint.h>

#include "kernel/pins.hpp"
#include "kernel/events.hpp"

#ifndef POT_EVENT_THRESHOLD
#define POT_EVENT_THRESHOLD 0.03
#endif

struct PotentiometerEvent
{
    double value;
};

class Potentiometer : public SyncEventSource<PotentiometerEvent>
{
private:
    AnalogInputPin pin;
    double value;

    
protected:
    double readPin();
    void generateEvents() override;

public:
    Potentiometer(uint8_t pin, EventFamily family);
    void begin(EventsManager *eventsManager) override;
};
