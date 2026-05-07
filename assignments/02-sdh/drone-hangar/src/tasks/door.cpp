#include "door.hpp"

#include "config.h"

DoorTask::ClosedState DoorTask::CLOSED;
BlockedTaskState<DoorTask> DoorTask::BLOCKED_CLOSED(&DoorTask::CLOSED);
DoorTask::OpenState DoorTask::OPEN;

DoorTask::DoorTask(ServoMotor *servo, MessageService *messageService)
    : Task<DoorTask>(&DoorTask::CLOSED),
      servo(servo),
      messageService(messageService) {}

void DoorTask::ClosedState::step(DoorTask *task, Context *context)
{
  blockOnAlarm(task, context, &DoorTask::BLOCKED_CLOSED);

  switch (context->getState())
  {
  case GlobalState::Takeoff:
  case GlobalState::Landing:
    task->switchState(&DoorTask::OPEN);
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
    if (task->servo->getAngle() >= DOOR_CLOSED_ANGLE + DOOR_ANGLE_MARGIN)
    {
      task->servo->setAngle(DOOR_CLOSED_ANGLE);
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
    if (task->servo->getAngle() <= DOOR_OPEN_ANGLE - DOOR_ANGLE_MARGIN)
    {
      task->servo->setAngle(DOOR_OPEN_ANGLE);
    }
    break;
  default:
    task->switchState(&DoorTask::CLOSED);
    break;
  }
}
