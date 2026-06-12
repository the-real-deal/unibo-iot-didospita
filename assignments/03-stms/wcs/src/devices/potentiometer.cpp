#include "potentiometer.hpp"

#include "servo.hpp"

#include <Arduino.h>

Potentiometer::Potentiometer(uint8_t pin, EventFamily family)
    : SyncEventSource(family), pin(pin), value(NAN) {}

void Potentiometer::begin(EventsManager *eventsManager)
{
    EventSource<PotentiometerEvent>::begin(eventsManager);
    this->pin.setup();
}

double Potentiometer::readPin()
{
    noInterrupts();
    double value = this->pin.read();
    interrupts();
    return value;
}

void Potentiometer::checkEvents()
{
    double value = this->readPin();
    if (abs(this->value - value) < POT_EVENT_THRESHOLD)
    {
        return;
    }
    this->value = value;
    this->generateEvent({.value = value});
}

double Potentiometer::getValue() { return this->value; }
