#include "door.hpp"

DoorTask::DoorTask(ServoMotor *servo, int closedAngle, int openAngle,
                   int angleMargin, MessageService *messageService)
    : Task<DoorTask>(&this->closedState),
      servo(servo),
      closedAngle(closedAngle),
      openAngle(openAngle),
      angleMargin(angleMargin),
      messageService(messageService),
      closedState(),
      blockedClosedState(&this->closedState),
      openState() {}

void DoorTask::ClosedState::step(DoorTask *task, Context *context)
{
  blockOnAlarm(task, context, &task->blockedClosedState);

  switch (context->getState())
  {
  case GlobalState::Takeoff:
  case GlobalState::Landing:
    task->switchState(&task->openState);
    break;
  case GlobalState::Inside:
    if (task->messageService->messageAvailable() &&
        task->messageService->getMessage()->getType() ==
            MessageType::REQUEST_TAKEOFF)
    {
      context->setState(GlobalState::Takeoff);
    }
    // no break, do default behaviour
  default:
    if (task->servo->getAngle() >= task->closedAngle + task->angleMargin)
    {
      task->servo->setAngle(task->closedAngle);
    }
    break;
  }
}

void DoorTask::OpenState::step(DoorTask *task, Context *context)
{
  switch (context->getState())
  {
  case GlobalState::Takeoff:
  case GlobalState::Landing:
    if (task->servo->getAngle() <= task->openAngle - task->angleMargin)
    {
      task->servo->setAngle(task->openAngle);
    }
    break;
  default:
    task->switchState(&task->closedState);
    break;
  }
}
