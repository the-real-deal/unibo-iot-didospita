#include "door.hpp"

#include "blocking.hpp"

DoorTask::DoorTask(ServoMotor* servo, int closedAngle, int openAngle,
                   MessageService* messageService)
    : Task<DoorTask>(new ClosedState()),
      servo(servo),
      closedAngle(closedAngle),
      openAngle(openAngle),
      messageService(messageService) {
  servo->setAngle(closedAngle);
}

void DoorTask::ClosedState::step(DoorTask* task, Context* context) {
  blockOnAlarm<DoorTask, ClosedState>(task, context);

  switch (context->getState()) {
    case GlobalState::Takeoff:
    case GlobalState::Landing:
      task->switchState(new OpenState());
      return;
    case GlobalState::Inside:
      if (task->messageService->messageAvailable() &&
          task->messageService->getMessage()->getType() ==
              MessageType::REQUEST_TAKEOFF) {
        context->setState(GlobalState::Takeoff);
      }
      // no break, perform angle read even if inside
    default:
      if (task->servo->getAngle() > task->closedAngle) {
        task->servo->setAngle(task->closedAngle);
      }
      break;
  }
}

void DoorTask::OpenState::step(DoorTask* task, Context* context) {
  switch (context->getState()) {
    case GlobalState::Takeoff:
    case GlobalState::Landing:
      if (task->servo->getAngle() < task->openAngle) {
        task->servo->setAngle(task->openAngle);
      }
      break;
    default:
      task->switchState(new ClosedState());
      break;
  }
}
