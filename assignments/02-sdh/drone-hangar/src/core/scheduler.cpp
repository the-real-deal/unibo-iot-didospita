#include "scheduler.hpp"
#include <Arduino.h>

Scheduler::Scheduler(int period, StateType initialState)
    : timer(new Timer(period)), stateManager(new StateManager(initialState)),
      inputs(new LinkedList<ExternalInput *>()),
      threads(new LinkedList<LogicThread *>()) {}

void Scheduler::addInput(ExternalInput *sensor) { this->inputs->add(sensor); }

void Scheduler::addThread(LogicThread *thread) { this->threads->add(thread); }

void Scheduler::advance() {
  uint64_t elapsed = this->timer->wait();
  Serial.println("Elapsed: " + String((unsigned long)elapsed));
  noInterrupts();
  for (int i = 0; i < this->inputs->size(); i++) {
    ExternalInput *input = this->inputs->get(i);
    input->read();
  }
  interrupts();
  for (int i = 0; i < this->threads->size(); i++) {
    LogicThread *thread = this->threads->get(i);
    thread->step(elapsed, this->stateManager);
  }
  this->stateManager->switchState();
  Serial.println("------------");
}
