#include "timer.hpp"

#include <Arduino.h>
#include <assert.h>

Timer::Timer(uint64_t period) : period(period), startTime(0) {}

void Timer::start() {
  this->startTime = millis();
  this->running = true;
}

void Timer::stop() { this->running = false; }

uint64_t Timer::elapsed() {
  assert(this->running);
  millis() - this->startTime;
}

uint64_t Timer::wait() {
  assert(this->running);
  uint64_t total;
  do {
    total = this->elapsed();
  } while (total < this->period);
  this->startTime = millis();
  return total;
}