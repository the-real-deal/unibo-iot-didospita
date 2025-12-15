#include "blink.hpp"

BlinkTask::BlinkTask(Indicator *blinkIndicator, uint64_t periodMillis)
    : Task<BlinkTask>(new IdleState()), blinkIndicator(blinkIndicator),
      periodMillis(periodMillis) {}

void BlinkTask::IdleState::step(BlinkTask *task, Context *context) {
  if (task->blinkIndicator->isOn()) {
    task->blinkIndicator->turnOff();
  }
  switch (context->getState()) {
  case GlobalState::Takeoff:
  case GlobalState::Landing:
    task->switchState(new OnState(task));
    break;
  default:
    break;
  }
}

BlinkTask::OnState::OnState(BlinkTask *task) : timer(task->periodMillis) {}

void BlinkTask::OnState::step(BlinkTask *task, Context *context) {
  if (task->blinkIndicator->isOff()) {
    task->blinkIndicator->turnOn();
  }
  switch (context->getState()) {
  case GlobalState::Takeoff:
  case GlobalState::Landing:
    if (this->timer.isFinished()) {
      task->switchState(new OffState(task));
    }
    break;
  default:
    task->switchState(new IdleState());
    break;
  }
}

BlinkTask::OffState::OffState(BlinkTask *task) : timer(task->periodMillis) {}

void BlinkTask::OffState::step(BlinkTask *task, Context *context) {
  if (task->blinkIndicator->isOn()) {
    task->blinkIndicator->turnOff();
  }
  switch (context->getState()) {
  case GlobalState::Takeoff:
  case GlobalState::Landing:
    if (this->timer.isFinished()) {
      task->switchState(new OnState(task));
    }
    break;
  default:
    task->switchState(new IdleState());
    break;
  }
}
