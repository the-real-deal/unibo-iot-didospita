#pragma once

#include "core/pins.hpp"
#include "core/scheduler.hpp"
#include "io/distance.hpp"
#include "io/temperature.hpp"

class UltrasonicSensor : public DistanceSensor, public ExternalInput {
private:
  DigitalInputPin echoPin;
  DigitalOutputPin triggerPin;
  uint64_t readDelayMicros;
  TemperatureSensor *temperatureSensor;
  float staticTemperature;
  float distance;

  float getTemperature();
  float getSoundSpeed();
  float pulseToDistance(float pulse);

protected:
  void read() override;

public:
  UltrasonicSensor(uint8_t echoPin, uint8_t triggerPin,
                   uint64_t readDelayMicros,
                   TemperatureSensor *temperatureSensor);
  UltrasonicSensor(uint8_t echoPin, uint8_t triggerPin,
                   uint64_t readDelayMicros, float temperature);
  float getDistance() override;
  bool isOutOfRange() override;
};
