#include "reset.hpp"

ResetTask::ResetTask(Button* resetButton, GlobalState initialState)
    : Task<ResetTask>(new IdleState()),
      resetButton(resetButton),
      prevState(initialState) {}

void ResetTask::IdleState::step(ResetTask* task, Context* context) {
  GlobalState state = context->getState();
  switch (state) {
    case GlobalState::Prealarm:
      break;
    case GlobalState::Alarm:
      task->switchState(new WaitResetState());
      break;
    default:
      task->prevState = state;
      break;
  }
}

void ResetTask::WaitResetState::step(ResetTask* task, Context* context) {
  if (task->resetButton->isPressed()) {
    context->setState(task->prevState);
    task->switchState(new IdleState());
  }
}