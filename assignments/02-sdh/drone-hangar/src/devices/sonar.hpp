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
  TemperatureSensor *tempSensor;
  uint32_t readStartMicros;
  uint32_t readDelayMicros;
  uint32_t readTimeoutMicros;
  uint32_t readOORMarginMicros;
  float minDistanceMm;
  float maxDistanceMm;
  float distanceMm;

  float getSoundSpeed();
  float pulseToDistance(uint32_t pulse);

protected:
  void read() override;

public:
  UltrasonicSensor(uint8_t echoPin, uint8_t triggerPin, TemperatureSensor *tempSensor,
                   uint32_t readStartMicros, uint32_t readDelayMicros,
                   uint32_t readTimeoutMicros, uint32_t readOORMarginMicros,
                   float minDistanceMm, float maxDistanceMm);

  void setup() override;
  float getDistanceMm() override;
};
