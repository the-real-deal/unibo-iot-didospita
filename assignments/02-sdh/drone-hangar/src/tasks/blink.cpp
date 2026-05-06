#include "blink.hpp"

BlinkTask::IdleState BlinkTask::IDLE;
BlinkTask::OnState BlinkTask::ON;
BlinkTask::OffState BlinkTask::OFF;

BlinkTask::BlinkTask(Indicator *blinkIndicator, uint64_t periodMillis)
    : Task<BlinkTask>(&BlinkTask::IDLE),
      blinkIndicator(blinkIndicator),
      periodMillis(periodMillis),
      timer() {}

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
    task->switchState(&BlinkTask::ON);
    break;
  default:
    break;
  }
}

void BlinkTask::OnState::setup(BlinkTask *task)
{
  task->timer = Timer(task->periodMillis);
  task->timer.start();
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
    if (task->timer.isFinished())
    {
      task->switchState(&BlinkTask::OFF);
    }
    break;
  default:
    task->switchState(&BlinkTask::IDLE);
    break;
  }
}

void BlinkTask::OffState::setup(BlinkTask *task)
{
  task->timer = Timer(task->periodMillis);
  task->timer.start();
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
    if (task->timer.isFinished())
    {
      task->switchState(&BlinkTask::ON);
    }
    break;
  default:
    task->switchState(&BlinkTask::IDLE);
    break;
  }
}
