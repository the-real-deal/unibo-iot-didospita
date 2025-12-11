#include "door.hpp"
#include "blocking.hpp"

DoorTask::DoorTask(ServoMotor *servo, int closedAngle, int openAngle)
    : Task<DoorTask>(new DoorTask::CloseAction()), servo(servo),
      closedAngle(closedAngle), openAngle(openAngle) {
  servo->setAngle(closedAngle);
}

void DoorTask::CloseAction::step(DoorTask *task, uint64_t elapsedTime,
                                 StateManager *stateManager) {
  blockOnAlarm(task, stateManager, new DoorTask::CloseAction());
  Serial.print("Door close: ");
  Serial.println(task->servo->getAngle());
  if (stateManager->getState() == StateType::TAKEOFF ||
      stateManager->getState() == StateType::LANDING) {
    task->switchAction(new DoorTask::OpenAction());
  } else if (task->servo->getAngle() > task->closedAngle) {
    task->servo->setAngle(task->closedAngle);
  }
}

void DoorTask::OpenAction::step(DoorTask *task, uint64_t elapsedTime,
                                StateManager *stateManager) {
  Serial.print("Door open: ");
  Serial.println(task->servo->getAngle());
  if (stateManager->getState() != StateType::TAKEOFF &&
      stateManager->getState() != StateType::LANDING) {
    task->switchAction(new DoorTask::CloseAction());
  } else if (task->servo->getAngle() < task->openAngle) {
    task->servo->setAngle(task->openAngle);
  }
}
