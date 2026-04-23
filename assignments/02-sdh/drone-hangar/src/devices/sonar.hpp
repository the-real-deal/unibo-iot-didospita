#pragma once

#include "core/pins.hpp"
#include "core/scheduler.hpp"
#include "io/distance.hpp"
#include "io/temperature.hpp"

class UltrasonicSensor : public DistanceSensor, public ExternalInput
{
private:
  DigitalInputPin echoPin;
  DigitalOutputPin triggerPin;
  uint64_t readStartMicros;
  uint64_t readDelayMicros;
  uint64_t readTimeoutMicros;
  TemperatureSensor *tempSensor;
  float minDistanceMm;
  float maxDistanceMm;
  float distanceMm;

  float getSoundSpeed();
  float pulseToDistance(uint64_t pulse);

protected:
  void read() override;

public:
  UltrasonicSensor(DigitalInputPin echoPin, DigitalOutputPin triggerPin,
                   uint64_t readStartMicros, uint64_t readDelayMicros,
                   uint64_t readTimeoutMicros, TemperatureSensor *tempSensor,
                   float minDistanceMm, float maxDistanceMm);
  float getDistanceMm() override;
};
