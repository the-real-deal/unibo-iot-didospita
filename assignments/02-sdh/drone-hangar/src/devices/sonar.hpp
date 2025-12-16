#pragma once

#include "core/pins.hpp"
#include "core/scheduler.hpp"
#include "io/distance.hpp"
#include "io/temperature.hpp"

class UltrasonicSensor : public DistanceSensor, public ExternalInput {
 private:
  // 'constexpr' needed for in-class initialization of static data member 'const
  // float UltrasonicSensor::DISTANCE_OOO_MIN' of non-integral type
  // [-fpermissive]
  static constexpr float DISTANCE_OOO_MIN = 0;
  static constexpr float DISTANCE_OOO_MAX = INFINITY;

  DigitalInputPin echoPin;
  DigitalOutputPin triggerPin;
  uint64_t readTimeoutMicros;
  uint64_t readDelayMicros;
  TemperatureSensor* tempSensor;
  float staticTemperature;
  float distance;

  float getTemperature();
  float getSoundSpeed();
  float pulseToDistance(uint64_t pulse);

 protected:
  void read() override;

 public:
  UltrasonicSensor(uint8_t echoPin, uint8_t triggerPin,
                   uint64_t readTimeoutMicros, uint64_t readDelayMicros,
                   TemperatureSensor* tempSensor);
  UltrasonicSensor(uint8_t echoPin, uint8_t triggerPin,
                   uint64_t readTimeoutMicros, uint64_t readDelayMicros,
                   float temperature);
  float getDistance() override;
};
