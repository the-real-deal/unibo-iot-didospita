#include "sonar.hpp"

#include <limits.h>

UltrasonicSensor::UltrasonicSensor(uint8_t echoPin, uint8_t triggerPin,
                                   uint64_t readStartMicros,
                                   uint64_t readDelayMicros,
                                   uint64_t readTimeoutMicros,
                                   TemperatureSensor *tempSensor,
                                   float maxDistanceMm)
    : echoPin(echoPin),
      triggerPin(triggerPin),
      readStartMicros(readStartMicros),
      readDelayMicros(readDelayMicros),
      readTimeoutMicros(readTimeoutMicros),
      tempSensor(tempSensor),
      maxDistanceMm(maxDistanceMm),
      distanceMm(0) {}

float UltrasonicSensor::getSoundSpeed()
{
  return 331.5 + 0.6 * this->tempSensor->getTemperature();
}

// https://github.com/pslab-unibo/esiot-2025-2026/blob/18463be0de31a90caf9740de2df1c6108cb350b3/lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/devices/Sonar.cpp#L29-L30
float UltrasonicSensor::pulseToDistance(uint64_t pulse)
{
  return ((float)pulse) / 1000.0 / 2 * this->getSoundSpeed(); // one less x/1000 to get mm
}

void UltrasonicSensor::read()
{
  this->triggerPin.write(DigitalValue::Low);
  delayMicroseconds(this->readStartMicros);
  this->triggerPin.write(DigitalValue::High);
  delayMicroseconds(this->readDelayMicros);
  this->triggerPin.write(DigitalValue::Low);

  uint64_t readStart = micros();
  uint64_t pulse =
      this->echoPin.readPulse(DigitalValue::High, this->readTimeoutMicros);
  uint64_t readTime = micros() - readStart;
  if (pulse == 0)
  {
    this->distanceMm = readTime < this->readTimeoutMicros ? 0 : this->maxDistanceMm;
  }
  else
  {
    this->distanceMm = this->pulseToDistance(pulse);
  }
}

float UltrasonicSensor::getDistanceMm()
{
  return this->distanceMm;
}
