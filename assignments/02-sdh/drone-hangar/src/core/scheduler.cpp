#include "scheduler.hpp"

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
    : context(periodMillis, initialState), inputCount(0), threadCount(0) {}

void Scheduler::addInput(ExternalInput* sensor) {
  if (inputCount < MAX_INPUTS) {
    inputs[inputCount] = sensor;
    inputCount++;
  }
}

void Scheduler::addThread(LogicThread* thread) {
  if (threadCount < MAX_THREADS) {
    threads[threadCount] = thread;
    threadCount++;
  }
}

void Scheduler::advance()
{
  this->context.waitTimer();
  for (int i = 0; i < inputCount; i++)
  {
    auto input = inputs[i];
    auto disableInterrupts = !input->requireInterrupts();
    if (disableInterrupts) {
      noInterrupts();
    }
    inputs[i]->read();
    if (disableInterrupts) {
      interrupts();
    }
  }
  for (int i = 0; i < threadCount; i++)
  {
    threads[i]->step(&this->context);
  }
  this->context.switchState();
}
