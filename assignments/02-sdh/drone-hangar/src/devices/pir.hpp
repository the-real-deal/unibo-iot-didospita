#pragma once

#include "core/pins.hpp"
#include "core/scheduler.hpp"
#include "io/presence.hpp"

class PIRSensor : public PresenceSensor, public ExternalInput
{
private:
  DigitalInputPin pin;
  DigitalValue value;

protected:
  void read() override;

public:
  PIRSensor(DigitalInputPin pin);
  bool isPresent() override;
};
