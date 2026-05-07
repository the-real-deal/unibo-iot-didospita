#include "stateChange.hpp"

#include "std/enum.hpp"
#include "std/debug.hpp"

StateChangeTask::IdleState StateChangeTask::IDLE;
BlockedTaskState<StateChangeTask> StateChangeTask::BLOCKED_IDLE(&StateChangeTask::IDLE);
StateChangeTask::PrintState StateChangeTask::PRINT;

StateChangeTask::StateChangeTask(Display *internalDisplay,
                                 MessageService *messageService)
    : Task<StateChangeTask>(&StateChangeTask::PRINT),
      internalDisplay(internalDisplay),
      messageService(messageService),
      prevState(GlobalState::Outside) {}

void StateChangeTask::IdleState::step(StateChangeTask *task, Context *context)
{
  blockOnAlarm(task, context, &StateChangeTask::BLOCKED_IDLE);
  GlobalState state = context->getState();
  switch (state)
  {
  case GlobalState::Prealarm:
    break;
  default:
    if (task->prevState != state)
    {
      task->switchState(&StateChangeTask::PRINT);
    }
    break;
  }
}

void StateChangeTask::PrintState::step(StateChangeTask *task,
                                       Context *context)
{
  GlobalState state = context->getState();
  const char *stateString = enumToString(state, GLOBAL_STATE_STRINGS);

  task->internalDisplay->clear();
  task->internalDisplay->print(stateString);
  task->messageService->send(Message(MessageType::STATE, stateString));
  task->prevState = state;
  task->switchState(&StateChangeTask::IDLE);
}
