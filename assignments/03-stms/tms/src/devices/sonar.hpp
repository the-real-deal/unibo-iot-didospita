#pragma once

#include "kernel/pins.hpp"
#include "kernel/events.hpp"

#ifndef SONAR_TEMP_CELSIUS
#define SONAR_TEMP_CELSIUS 20
#endif

#ifndef SONAR_READ_START_US
#define SONAR_READ_START_US 3
#endif

#ifndef SONAR_READ_DELAY_US
#define SONAR_READ_DELAY_US 5
#endif

#ifndef SONAR_READ_TIMEOUT_US
#define SONAR_READ_TIMEOUT_US (30L * 1000L) // 30ms =~ 5m
#endif

#ifndef SONAR_READ_OOR_MARGIN_US
#define SONAR_READ_OOR_MARGIN_US (5L * 1000L) // pulseIn takes slightly lower than the timeout when out of range
#endif

#ifndef SONAR_MIN_DISTANCE_MM
#define SONAR_MIN_DISTANCE_MM 50
#endif

#ifndef SONAR_MAX_DISTANCE_MM
#define SONAR_MAX_DISTANCE_MM 3000
#endif

struct SonarEvent
{
  float distanceMm;
};

class Sonar : public SyncEventSource<SonarEvent>
{
private:
  DigitalInputPin echoPin;
  DigitalOutputPin triggerPin;

  float getSoundSpeed();
  float pulseToDistance(uint32_t pulse);
  float read();

protected:
    void generateEvents() override;

public:
  Sonar(uint8_t echoPin, uint8_t triggerPin, EventFamily sonarEventFamily);
  void begin(EventsManager *eventsManager) override;
};
