#include "reset.hpp"

ResetTask::IdleState ResetTask::IDLE;
ResetTask::WaitResetState ResetTask::WAIT_RESET;

ResetTask::ResetTask(Button *resetButton, GlobalState initialState)
    : Task<ResetTask>(&ResetTask::IDLE),
      resetButton(resetButton),
      prevState(initialState) {}

void ResetTask::IdleState::step(ResetTask *task, Context *context)
{
  GlobalState state = context->getState();
  switch (state)
  {
  case GlobalState::Prealarm:
    break;
  case GlobalState::Alarm:
    task->switchState(&ResetTask::WAIT_RESET);
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
    task->switchState(&ResetTask::IDLE);
  }
}