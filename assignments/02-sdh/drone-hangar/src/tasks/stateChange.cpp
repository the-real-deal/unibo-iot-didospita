#include "stateChange.hpp"

#include "std/enum.hpp"

StateChangeTask::StateChangeTask(Display *internalDisplay,
                                 MessageService *messageService)
    : Task<StateChangeTask>(&this->printState),
      internalDisplay(internalDisplay),
      messageService(messageService),
      prevState(GlobalState::Outside),
      idleState(),
      blockedIdleState(&this->idleState),
      printState() {}

void StateChangeTask::IdleState::step(StateChangeTask *task, Context *context)
{
  blockOnAlarm(task, context, &task->blockedIdleState);
  GlobalState state = context->getState();
  switch (state)
  {
  case GlobalState::Prealarm:
    break;
  default:
    if (task->prevState != state)
    {
      task->switchState(&task->printState);
    }
    break;
  }
}

void StateChangeTask::PrintState::step(StateChangeTask *task,
                                       Context *context)
{
  GlobalState state = context->getState();
  auto stateString = enumToString(state, GLOBAL_STATE_STRINGS);

  task->internalDisplay->clear();
  task->internalDisplay->print(stateString);
  task->messageService->send(Message(MessageType::STATE, stateString));
  task->prevState = state;
  task->switchState(&task->idleState);
}
