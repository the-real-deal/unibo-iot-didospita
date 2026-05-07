#include "scheduler.hpp"

#include "std/enum.hpp"
#include "std/debug.hpp"

Context::Context(GlobalState initialState)
    : state(initialState),
      stateCandidate(initialState),
      elapsedMillis(0) {}

GlobalState Context::getState() { return this->state; }

void Context::setState(GlobalState state) { this->stateCandidate = state; }

uint32_t Context::getElapsedMillis() { return this->elapsedMillis; }

Scheduler::Scheduler(int periodMillis, GlobalState initialState)
    : context(initialState), timer(periodMillis), inputs(), n_inputs(0), threads(), n_threads(0) {}

void Scheduler::addInput(ExternalInput *input)
{
  if (this->n_inputs >= SCHEDULER_MAX_INPUTS)
  {
    return;
  }
  this->inputs[this->n_inputs] = input;
  this->n_inputs++;
}

void Scheduler::addThread(LogicThread *thread)
{
  if (this->n_threads >= SCHEDULER_MAX_THREADS)
  {
    return;
  }
  this->threads[this->n_threads] = thread;
  this->n_threads++;
}

void Scheduler::setup()
{
  for (size_t i = 0; i < this->n_inputs; i++)
  {
    ExternalInput *input = this->inputs[i];
    input->setup();
  }
  for (size_t i = 0; i < this->n_threads; i++)
  {
    LogicThread *thread = this->threads[i];
    thread->setup();
  }
  this->timer.start();
}

void Scheduler::advance()
{
  this->context.elapsedMillis = this->timer.wait();

  for (size_t i = 0; i < this->n_inputs; i++)
  {
    ExternalInput *input = this->inputs[i];
    input->read();
  }
  for (size_t i = 0; i < this->n_threads; i++)
  {
    LogicThread *thread = this->threads[i];
    thread->step(&this->context);
  }

  this->context.state = this->context.stateCandidate;
  this->timer.start();
}
