#include "scheduler.hpp"

#include "std/enum.hpp"

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
    : context(periodMillis, initialState), inputs(), n_inputs(0), threads(), n_threads(0) {}

void Scheduler::addInput(ExternalInput *input)
{
  if (this->n_inputs >= SCHEDULER_MAX_INPUTS) {
    return;
  }
  this->inputs[this->n_inputs] = input;
  this->n_inputs++;
}

void Scheduler::addThread(LogicThread *thread)
{
  if (this->n_threads >= SCHEDULER_MAX_THREADS) {
    return;
  }
  this->threads[this->n_threads] = thread;
  this->n_threads++;
}

void Scheduler::advance()
{
  this->context.waitTimer();

  for (int i = 0; i < this->n_inputs; i++)
  {
    auto input = this->inputs[i];
    input->read();
  }
  for (int i = 0; i < this->n_threads; i++)
  {
    auto thread = this->threads[i];
    thread->step(&this->context);
  }
  this->context.switchState();
}
