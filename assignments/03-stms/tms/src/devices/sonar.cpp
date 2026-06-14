#include "sonar.hpp"

#include <Arduino.h>
#include <limits.h>

#include "config.h"

Sonar::Sonar(uint8_t echoPin, uint8_t triggerPin, EventFamily sonarEventFamily)
    : SyncEventSource(sonarEventFamily),
      echoPin(echoPin),
      triggerPin(triggerPin) {}

void Sonar::begin(EventsManager *eventsManager)
{
  SyncEventSource<SonarEvent>::begin(eventsManager);
  this->echoPin.setup();
  this->triggerPin.setup();
}

float Sonar::getSoundSpeed()
{
  float soundSpeed = 331.5 + 0.6 * SONAR_TEMP_CELSIUS;
  return soundSpeed;
}

// https://github.com/pslab-unibo/esiot-2025-2026/blob/18463be0de31a90caf9740de2df1c6108cb350b3/lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/devices/Sonar.cpp#L29-L30
float Sonar::pulseToDistance(uint32_t pulse)
{
  return ((float)pulse) / 1000.0 / 2 * this->getSoundSpeed(); // one less x/1000 to get mm
}

float Sonar::read()
{
  noInterrupts();
  this->triggerPin.write(DigitalValue::Low);
  delayMicroseconds(SONAR_READ_START_US);
  this->triggerPin.write(DigitalValue::High);
  delayMicroseconds(SONAR_READ_DELAY_US);
  this->triggerPin.write(DigitalValue::Low);

  uint32_t readStart = micros();
  uint32_t pulse =
      this->echoPin.readPulse(DigitalValue::High, SONAR_READ_TIMEOUT_US);
  uint32_t readTime = micros() - readStart;
  interrupts();
  if (pulse == 0)
  {
    return readTime < (SONAR_READ_TIMEOUT_US - SONAR_READ_OOR_MARGIN_US) ? SONAR_MIN_DISTANCE_MM : SONAR_MAX_DISTANCE_MM;
  }
  else
  {
    float distance = this->pulseToDistance(pulse);
    return min((float)SONAR_MAX_DISTANCE_MM, (float)max(distance, (float)SONAR_MIN_DISTANCE_MM));
  }
}

void Sonar::generateEvents()
{
  Serial.println(F("SONAR GENEVENTS"));
  Serial.flush();
  float distanceMm = this->read();
  this->generateEvent({.distanceMm = distanceMm});
}
