#include "reset.hpp"

ResetTask::ResetTask(Button *resetButton, GlobalState initialState)
    : Task<ResetTask>(&this->idleState),
      resetButton(resetButton),
      prevState(initialState),
      idleState(),
      waitResetState() {}

void ResetTask::IdleState::step(ResetTask *task, Context *context)
{
  GlobalState state = context->getState();
  switch (state)
  {
  case GlobalState::Prealarm:
    break;
  case GlobalState::Alarm:
    task->switchState(&task->waitResetState);
    break;
  default:
    task->prevState = state;
    break;
  }
}

void ResetTask::WaitResetState::step(ResetTask *task, Context *context)
{
  if (task->resetButton->isPressed())
  {
    context->setState(task->prevState);
    task->switchState(&task->idleState);
  }
}