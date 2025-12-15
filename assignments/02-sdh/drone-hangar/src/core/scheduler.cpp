#include "scheduler.hpp"
#include "utils.hpp"
#include <Arduino.h>

Scheduler::Scheduler(int period, GlobalState initialState)
    : context(period, initialState), inputs(), threads() {}

void Scheduler::addInput(ExternalInput *sensor) { this->inputs.add(sensor); }

void Scheduler::addThread(LogicThread *thread) { this->threads.add(thread); }

void Scheduler::advance() {
  this->context.waitTimer();
  Serial.print("Elapsed: ");
  Serial.println((unsigned long)this->context.getElapsedMillis());
  Serial.print("State: ");
  Serial.println(enumToString(this->context.getState(), STATE_TYPE_STRINGS));
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
