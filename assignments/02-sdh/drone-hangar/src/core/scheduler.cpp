#include "scheduler.hpp"
#include "utils.hpp"
#include <Arduino.h>

Scheduler::Scheduler(int period, StateType initialState)
    : timer(period), stateManager(initialState), inputs(), threads() {}

void Scheduler::addInput(ExternalInput *sensor) { this->inputs.add(sensor); }

void Scheduler::addThread(LogicThread *thread) { this->threads.add(thread); }
void Scheduler::advance() {
  uint64_t elapsed = this->timer.wait();
  Serial.print("Elapsed: ");
  Serial.println((unsigned long)elapsed);
  Serial.print("State: ");
  Serial.println(
      enumToString(this->stateManager.getState(), STATE_TYPE_STRINGS));
  noInterrupts();
  for (int i = 0; i < this->inputs.size(); i++) {
    ExternalInput *input = this->inputs.get(i);
    input->read();
  }
  interrupts();
  for (int i = 0; i < this->threads.size(); i++) {
    LogicThread *thread = this->threads.get(i);
    thread->step(elapsed, &this->stateManager);
  }
  this->stateManager.switchState();
  Serial.println("------------");
}
