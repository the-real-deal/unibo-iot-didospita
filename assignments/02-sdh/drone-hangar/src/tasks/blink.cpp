#include "blink.hpp"

BlinkTask::BlinkTask(Indicator *blinkIndicator, uint64_t periodMillis)
    : Task<BlinkTask>(&this->idleState),
      blinkIndicator(blinkIndicator),
      periodMillis(periodMillis),
      idleState(),
      onState(this),
      offState(this) {}

void BlinkTask::IdleState::step(BlinkTask *task, Context *context)
{
  if (task->blinkIndicator->isOn())
  {
    task->blinkIndicator->turnOff();
  }
  switch (context->getState())
  {
  case GlobalState::Takeoff:
  case GlobalState::Landing:
    task->switchState(&task->onState);
    break;
  default:
    break;
  }
}

BlinkTask::OnState::OnState(BlinkTask *task) : timer(task->periodMillis) {}

void BlinkTask::OnState::setup(BlinkTask *task)
{
  this->timer.start();
}

void BlinkTask::OnState::step(BlinkTask *task, Context *context)
{
  if (task->blinkIndicator->isOff())
  {
    task->blinkIndicator->turnOn();
  }
  switch (context->getState())
  {
  case GlobalState::Takeoff:
  case GlobalState::Landing:
    if (this->timer.isFinished())
    {
      task->switchState(&task->offState);
    }
    break;
  default:
    task->switchState(&task->idleState);
    break;
  }
}

BlinkTask::OffState::OffState(BlinkTask *task) : timer(task->periodMillis) {}

void BlinkTask::OffState::setup(BlinkTask *task)
{
  this->timer.start();
}

void BlinkTask::OffState::step(BlinkTask *task, Context *context)
{
  if (task->blinkIndicator->isOn())
  {
    task->blinkIndicator->turnOff();
  }
  switch (context->getState())
  {
  case GlobalState::Takeoff:
  case GlobalState::Landing:
    if (this->timer.isFinished())
    {
      task->switchState(&task->onState);
    }
    break;
  default:
    task->switchState(&task->idleState);
    break;
  }
}
