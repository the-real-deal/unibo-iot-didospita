#include "timer.hpp"

#include <Arduino.h>

Timer initTimer(const uint64_t period) {
  return Timer{
      .period = period,
      .startTime = millis(),
  };
}

bool timerEnded(const Timer *const timer) {
  return (millis() - timer->startTime) >= timer->period;
}
