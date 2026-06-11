#include "potentiometer.hpp"

#include "servo.hpp"

#include <Arduino.h>

Potentiometer::Potentiometer(uint8_t pin, EventFamily family, EventsManager *eventManager)
    : SyncEventSource(family, eventManager), pin(pin), value(NAN) {}

void Potentiometer::setup()
{
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
