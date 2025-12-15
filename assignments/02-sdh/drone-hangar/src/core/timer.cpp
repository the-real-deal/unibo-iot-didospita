#include "timer.hpp"

#include <Arduino.h>
#include <assert.h>

Timer::Timer(uint64_t periodMillis)
    : periodMillis(periodMillis), startTime(millis()) {}

uint64_t Timer::getElapsedMillis() { return millis() - this->startTime; }

bool Timer::isFinished() {
  return this->getElapsedMillis() >= this->periodMillis;
}

uint64_t Timer::wait() {
  while (!this->isFinished())
    ;
  uint64_t total = this->getElapsedMillis();
  this->startTime = millis();
  return total;
}