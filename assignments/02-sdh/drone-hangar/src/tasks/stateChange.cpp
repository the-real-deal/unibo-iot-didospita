#include "stateChange.hpp"

#include "blocking.hpp"
#include "utils.hpp"

StateChangeTask::StateChangeTask(Display* internalDisplay,
                                 MessageService* messageService,
                                 GlobalState initialState)
    : Task<StateChangeTask>(new PrintState()),
      internalDisplay(internalDisplay),
      messageService(messageService),
      prevState(initialState) {}

void StateChangeTask::IdleState::step(StateChangeTask* task, Context* context) {
  blockOnAlarm<StateChangeTask, IdleState>(task, context);
  GlobalState state = context->getState();
  switch (state) {
    case GlobalState::Prealarm:
      break;
    default:
      if (task->prevState != state) {
        task->switchState(new PrintState());
      }
      break;
  }
}

void StateChangeTask::PrintState::step(StateChangeTask* task,
                                       Context* context) {
  GlobalState state = context->getState();
  String stateString = String(enumToString(state, GLOBAL_STATE_STRINGS));
  task->internalDisplay->clear();
  task->internalDisplay->print(&stateString);
  task->messageService->send(Message(MessageType::STATE, stateString));
  task->prevState = state;
  task->switchState(new IdleState());
}
