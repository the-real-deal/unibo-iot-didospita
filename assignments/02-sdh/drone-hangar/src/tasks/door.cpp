#include "door.hpp"
#include "blocking.hpp"

DoorTask::DoorTask(ServoMotor *servo, int closedAngle, int openAngle)
    : Task<DoorTask>(new DoorTask::ClosedState()), servo(servo),
      closedAngle(closedAngle), openAngle(openAngle) {
  servo->setAngle(closedAngle);
}

void DoorTask::ClosedState::step(DoorTask *task, SchedulerContext *context) {
  blockOnAlarm(task, context, new DoorTask::ClosedState());

  switch (context->getState()) {
  case GlobalState::Takeoff:
  case GlobalState::Landing:
    task->switchTo(new DoorTask::OpenState());
    break;
  default:
    if (task->servo->getAngle() > task->closedAngle) {
      task->servo->setAngle(task->closedAngle);
    }
    break;
  }
}

void DoorTask::OpenState::step(DoorTask *task, SchedulerContext *context) {
  switch (context->getState()) {
  case GlobalState::Takeoff:
  case GlobalState::Landing:
    if (task->servo->getAngle() < task->openAngle) {
      task->servo->setAngle(task->openAngle);
    }
    break;
  default:
    task->switchTo(new DoorTask::ClosedState());
    break;
  }
}
