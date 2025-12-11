#include "door.hpp"
#include "blocking.hpp"

DoorTask::DoorTask(ServoMotor *servo, int closedAngle, int openAngle)
    : Task<DoorTask>(new DoorTask::CloseAction()), servo(servo),
      closedAngle(closedAngle), openAngle(openAngle) {
  servo->setAngle(closedAngle);
}

void DoorTask::CloseAction::step(DoorTask *task, SchedulerContext *context) {
  blockOnAlarm(task, context, new DoorTask::CloseAction());
  Serial.print("Door close: ");
  Serial.println(task->servo->getAngle());
  if (context->state() == StateType::Takeoff ||
      context->state() == StateType::Landing) {
    task->switchAction(new DoorTask::OpenAction());
  } else if (task->servo->getAngle() > task->closedAngle) {
    task->servo->setAngle(task->closedAngle);
  }
}

void DoorTask::OpenAction::step(DoorTask *task, SchedulerContext *context) {
  Serial.print("Door open: ");
  Serial.println(task->servo->getAngle());
  if (context->state() != StateType::Takeoff &&
      context->state() != StateType::Landing) {
    task->switchAction(new DoorTask::CloseAction());
  } else if (task->servo->getAngle() < task->openAngle) {
    task->servo->setAngle(task->openAngle);
  }
}
