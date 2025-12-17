#include "ddd.hpp"

#include "blocking.hpp"

DDDTask::DDDTask(DistanceSensor* droneDistanceSensor,
                 MessageService* messageService, float outsideDistance,
                 uint64_t outsideTimeMillis, float insideDistance,
                 uint64_t insideTimeMillis)
    : Task<DDDTask>(new IdleState()),
      droneDistanceSensor(droneDistanceSensor),
      messageService(messageService),
      outsideDistance(outsideDistance),
      outsideTimeMillis(outsideTimeMillis),
      insideDistance(insideDistance),
      insideTimeMillis(insideTimeMillis) {}

void DDDTask::sendDistance(float distance) {
  this->messageService->send(Message(MessageType::DISTANCE, String(distance)));
}

void DDDTask::IdleState::step(DDDTask* task, Context* context) {
  blockOnAlarm<DDDTask, IdleState>(task, context);

  switch (context->getState()) {
    case GlobalState::Takeoff:
      task->switchState(new TakeoffReadingState());
      break;
    case GlobalState::Landing:
      task->switchState(new LandingReadingState());
      break;
    default:
      break;
  }
}

void DDDTask::TakeoffReadingState::step(DDDTask* task, Context* context) {
  if (task->droneDistanceSensor->getDistance() >= task->outsideDistance) {
    task->switchState(new TakeoffDistanceCheckingState(task));
  }
}

DDDTask::TakeoffDistanceCheckingState::TakeoffDistanceCheckingState(
    DDDTask* task)
    : timer(task->outsideTimeMillis) {}

void DDDTask::TakeoffDistanceCheckingState::step(DDDTask* task,
                                                 Context* context) {
  if (this->timer.isFinished()) {
    context->setState(GlobalState::Outside);
    task->switchState(new IdleState());
  } else if (task->droneDistanceSensor->getDistance() < task->outsideDistance) {
    task->switchState(new TakeoffReadingState());
  }
}

void DDDTask::LandingReadingState::step(DDDTask* task, Context* context) {
  float distance = task->droneDistanceSensor->getDistance();
  task->sendDistance(distance);
  if (distance <= task->insideDistance) {
    task->switchState(new LandingDistanceCheckingState(task));
  }
}

DDDTask::LandingDistanceCheckingState::LandingDistanceCheckingState(
    DDDTask* task)
    : timer(task->insideTimeMillis) {}

void DDDTask::LandingDistanceCheckingState::step(DDDTask* task,
                                                 Context* context) {
  if (this->timer.isFinished()) {
    context->setState(GlobalState::Inside);
    task->switchState(new IdleState());
  } else {
    float distance = task->droneDistanceSensor->getDistance();
    task->sendDistance(distance);
    if (distance > task->insideDistance) {
      task->switchState(new LandingReadingState());
    }
  }
}
