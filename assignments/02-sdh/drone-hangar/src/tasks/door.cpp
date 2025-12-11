#include "door.hpp"
#include "blocking.hpp"

DoorTask::DoorTask(ServoMotor *servo, int closedAngle, int openAngle)
    : Task<DoorTask>(new DoorTask::ClosedState()), servo(servo),
      closedAngle(closedAngle), openAngle(openAngle) {
  servo->setAngle(closedAngle);
}

void DoorTask::ClosedState::step(DoorTask *task, SchedulerContext *context) {
  blockOnAlarm(task, context, new DoorTask::ClosedState());
  Serial.print("Door close: ");
  Serial.println(task->servo->getAngle());
  if (context->getState() == GlobalState::Takeoff ||
      context->getState() == GlobalState::Landing) {
    task->switchTo(new DoorTask::OpenState());
  } else if (task->servo->getAngle() > task->closedAngle) {
    task->servo->setAngle(task->closedAngle);
  }
}

void DoorTask::OpenState::step(DoorTask *task, SchedulerContext *context) {
  Serial.print("Door open: ");
  Serial.println(task->servo->getAngle());
  if (context->getState() != GlobalState::Takeoff &&
      context->getState() != GlobalState::Landing) {
    task->switchTo(new DoorTask::ClosedState());
  } else if (task->servo->getAngle() < task->openAngle) {
    task->servo->setAngle(task->openAngle);
  }
}
