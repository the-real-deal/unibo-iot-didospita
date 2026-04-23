#include "door.hpp"

#include "blocking.hpp"

DoorTask::DoorTask(ServoMotor *servo, int closedAngle, int openAngle,
                   int angleMargin, MessageService *messageService)
    : Task<DoorTask>(new ClosedState()),
      servo(servo),
      closedAngle(closedAngle),
      openAngle(openAngle),
      angleMargin(angleMargin),
      messageService(messageService) {}

void DoorTask::ClosedState::step(DoorTask *task, Context *context)
{
  blockOnAlarm<DoorTask, ClosedState>(task, context);

  switch (context->getState())
  {
  case GlobalState::Takeoff:
  case GlobalState::Landing:
    task->switchState(new OpenState());
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
    task->switchState(new ClosedState());
    break;
  }
}
