#include "timer.hpp"

#include <Arduino.h>
#include <assert.h>

#include "config.h"

Timer::Timer(uint64_t periodMillis) : periodMillis(periodMillis) {}
Timer::Timer() : Timer(0) {}

uint64_t Timer::getElapsedMillis() { return millis() - this->startTime; }

bool Timer::isFinished()
{
  return this->getElapsedMillis() >= this->periodMillis;
}

void Timer::start()
{
  this->startTime = millis();
}

uint64_t Timer::wait()
{
  while (this->getElapsedMillis() < this->periodMillis)
  {
    delay(TIMER_READ_FREQ_MS);
  }
  uint64_t total = this->getElapsedMillis();
  return total;
}