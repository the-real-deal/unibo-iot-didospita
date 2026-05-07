#include "sonar.hpp"

#include <limits.h>
#include <limits.h>

UltrasonicSensor::UltrasonicSensor(uint8_t echoPin, uint8_t triggerPin,
                                   uint32_t readStartMicros, uint32_t readDelayMicros,
                                   uint32_t readTimeoutMicros, TemperatureSensor *tempSensor,
                                   float minDistanceMm, float maxDistanceMm)
    : echoPin(echoPin),
      triggerPin(triggerPin),
      readStartMicros(readStartMicros),
      readDelayMicros(readDelayMicros),
      readTimeoutMicros(readTimeoutMicros),
      tempSensor(tempSensor),
      minDistanceMm(minDistanceMm),
      maxDistanceMm(maxDistanceMm),
      distanceMm(0) {}

void UltrasonicSensor::setup()
{
  this->echoPin.setup();
  this->triggerPin.setup();
}

float UltrasonicSensor::getSoundSpeed()
{
  float temp = this->tempSensor->getTemperature();
  float soundSpeed = 331.5 + 0.6 * temp;
  return soundSpeed;
}

// https://github.com/pslab-unibo/esiot-2025-2026/blob/18463be0de31a90caf9740de2df1c6108cb350b3/lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/devices/Sonar.cpp#L29-L30
float UltrasonicSensor::pulseToDistance(uint32_t pulse)
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

  uint32_t readStart = micros();
  uint32_t pulse =
      this->echoPin.readPulse(DigitalValue::High, this->readTimeoutMicros);
  uint32_t readTime = micros() - readStart;
  if (pulse == 0)
  {
    this->distanceMm = readTime < this->readTimeoutMicros ? this->minDistanceMm : this->maxDistanceMm;
  }
  else
  {
    float distance = this->pulseToDistance(pulse);
    this->distanceMm = min(this->maxDistanceMm, max(distance, this->minDistanceMm));
  }
}

float UltrasonicSensor::getDistanceMm()
{
  return this->distanceMm;
}
