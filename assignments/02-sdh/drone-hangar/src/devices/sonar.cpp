#include "sonar.hpp"

#include <limits.h>
#include <limits.h>

#include "config.h"

UltrasonicSensor::UltrasonicSensor(uint8_t echoPin, uint8_t triggerPin, TemperatureSensor *tempSensor)
    : echoPin(echoPin),
      triggerPin(triggerPin),
      tempSensor(tempSensor),
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
  delayMicroseconds(SONAR_READ_START_US);
  this->triggerPin.write(DigitalValue::High);
  delayMicroseconds(SONAR_READ_DELAY_US);
  this->triggerPin.write(DigitalValue::Low);

  uint32_t readStart = micros();
  uint32_t pulse =
      this->echoPin.readPulse(DigitalValue::High, SONAR_READ_TIMEOUT_US);
  uint32_t readTime = micros() - readStart;
  if (pulse == 0)
  {
    this->distanceMm = readTime < (SONAR_READ_TIMEOUT_US - SONAR_READ_OOR_MARGIN_US) ? SONAR_MIN_DISTANCE_MM : SONAR_MAX_DISTANCE_MM;
  }
  else
  {
    float distance = this->pulseToDistance(pulse);
    this->distanceMm = min(SONAR_MAX_DISTANCE_MM, max(distance, SONAR_MIN_DISTANCE_MM));
  }
}

float UltrasonicSensor::getDistanceMm()
{
  return this->distanceMm;
}
