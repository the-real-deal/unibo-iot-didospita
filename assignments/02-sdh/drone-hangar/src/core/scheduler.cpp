#include "scheduler.hpp"
#include "utils.hpp"
#include <Arduino.h>

Context::Context(int period, GlobalState initialState)
    : timer(period), state(initialState), previousState(initialState),
      elapsedMillis(0) {}

void Context::waitTimer() { this->elapsedMillis = this->timer.wait(); }

void Context::switchState() {
  if (this->stateCandidate != this->state) {
    this->previousState = this->state;
    this->state = this->stateCandidate;
  }
}

GlobalState Context::getState() { return this->state; }

GlobalState Context::getPreviousState() { return this->previousState; }

void Context::setState(GlobalState state) { this->stateCandidate = state; }

uint64_t Context::getElapsedMillis() { return this->elapsedMillis; }

Scheduler::Scheduler(int period, GlobalState initialState)
    : context(period, initialState), inputs(), threads() {}

void Scheduler::addInput(ExternalInput *sensor) { this->inputs.add(sensor); }

void Scheduler::addThread(LogicThread *thread) { this->threads.add(thread); }

void Scheduler::advance() {
  this->context.waitTimer();
  Serial.print("Elapsed: ");
  Serial.println((unsigned long)this->context.getElapsedMillis());
  Serial.print("State: ");
  Serial.println(enumToString(this->context.getState(), GLOBAL_STATE_STRINGS));
  noInterrupts();
  for (int i = 0; i < this->inputs.size(); i++) {
    ExternalInput *input = this->inputs.get(i);
    input->read();
  }
  interrupts();
  for (int i = 0; i < this->threads.size(); i++) {
    LogicThread *thread = this->threads.get(i);
    thread->step(&this->context);
  }
  this->context.switchState();
  Serial.println("------------");
}
