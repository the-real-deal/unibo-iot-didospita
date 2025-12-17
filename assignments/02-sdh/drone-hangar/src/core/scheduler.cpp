#include "scheduler.hpp"

#include <Arduino.h>

#include "utils.hpp"

Context::Context(int periodMillis, GlobalState initialState)
    : timer(periodMillis),
      state(initialState),
      stateCandidate(initialState),
      elapsedMillis(0) {}

void Context::waitTimer() { this->elapsedMillis = this->timer.wait(); }

void Context::switchState() { this->state = this->stateCandidate; }

GlobalState Context::getState() { return this->state; }

void Context::setState(GlobalState state) { this->stateCandidate = state; }

uint64_t Context::getElapsedMillis() { return this->elapsedMillis; }

Scheduler::Scheduler(int periodMillis, GlobalState initialState)
    : context(periodMillis, initialState), inputs(), threads() {}

void Scheduler::addInput(ExternalInput* sensor) { this->inputs.add(sensor); }

void Scheduler::addThread(LogicThread* thread) { this->threads.add(thread); }

void Scheduler::advance() {
  this->context.waitTimer();
  noInterrupts();
  for (int i = 0; i < this->inputs.size(); i++) {
    ExternalInput* input = this->inputs.get(i);
    input->read();
  }
  interrupts();
  for (int i = 0; i < this->threads.size(); i++) {
    LogicThread* thread = this->threads.get(i);
    thread->step(&this->context);
  }
  this->context.switchState();
}
