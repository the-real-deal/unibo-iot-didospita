#include "timer.hpp"

#include <Arduino.h>
#include <assert.h>

#include "config.h"

Timer::Timer(uint32_t periodMillis) : periodMillis(periodMillis), startTime(0) {}
Timer::Timer() : Timer(0) {}

uint32_t Timer::getElapsedMillis() { return millis() - this->startTime; }

bool Timer::isFinished()
{
  return this->getElapsedMillis() >= this->periodMillis;
}

void Timer::start()
{
  this->startTime = millis();
}

uint32_t Timer::wait()
{
  while (this->getElapsedMillis() < this->periodMillis)
  {
    delay(TIMER_READ_FREQ_MS);
  }
  uint32_t total = this->getElapsedMillis();
  return total;
}