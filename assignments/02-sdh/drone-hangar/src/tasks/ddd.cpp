#include "ddd.hpp"

DDDTask::DDDTask(DistanceSensor *droneDistanceSensor,
                 MessageService *messageService, float outsideDistance,
                 uint64_t outsideTimeMillis, float insideDistance,
                 uint64_t insideTimeMillis)
    : Task<DDDTask>(&this->idleState),
      droneDistanceSensor(droneDistanceSensor),
      messageService(messageService),
      outsideDistanceMm(outsideDistance),
      outsideTimeMillis(outsideTimeMillis),
      insideDistanceMm(insideDistance),
      insideTimeMillis(insideTimeMillis),
      idleState(),
      blockedIdleState(&this->idleState),
      takeoffReadingState(),
      takeoffDistanceCheckingState(this),
      landingReadingState(),
      landingDistanceCheckingState(this) {}

void DDDTask::sendDistance(float distance)
{
  this->messageService->send(Message(MessageType::DISTANCE, String(distance)));
}

void DDDTask::IdleState::step(DDDTask *task, Context *context)
{
  blockOnAlarm(task, context, &task->blockedIdleState);

  switch (context->getState())
  {
  case GlobalState::Takeoff:
    task->switchState(&task->takeoffReadingState);
    break;
  case GlobalState::Landing:
    task->switchState(&task->landingReadingState);
    break;
  default:
    break;
  }
}

void DDDTask::TakeoffReadingState::step(DDDTask *task, Context *context)
{
  auto distance = task->droneDistanceSensor->getDistanceMm();
  if (distance >= task->outsideDistanceMm)
  {
    task->switchState(&task->takeoffDistanceCheckingState);
  }
}

DDDTask::TakeoffDistanceCheckingState::TakeoffDistanceCheckingState(
    DDDTask *task)
    : timer(task->outsideTimeMillis) {}

void DDDTask::TakeoffDistanceCheckingState::setup(DDDTask *task)
{
  this->timer.start();
}

void DDDTask::TakeoffDistanceCheckingState::step(DDDTask *task,
                                                 Context *context)
{
  if (this->timer.isFinished())
  {
    context->setState(GlobalState::Outside);
    task->switchState(&task->idleState);
    return;
  }

  auto distance = task->droneDistanceSensor->getDistanceMm();
  if (distance < task->outsideDistanceMm)
  {
    task->switchState(&task->takeoffReadingState);
  }
}

void DDDTask::LandingReadingState::step(DDDTask *task, Context *context)
{
  auto distance = task->droneDistanceSensor->getDistanceMm();
  task->sendDistance(distance);
  if (distance <= task->insideDistanceMm)
  {
    task->switchState(&task->landingDistanceCheckingState);
  }
}

DDDTask::LandingDistanceCheckingState::LandingDistanceCheckingState(
    DDDTask *task)
    : timer(task->insideTimeMillis) {}

void DDDTask::LandingDistanceCheckingState::setup(DDDTask *task)
{
  this->timer.start();
}

void DDDTask::LandingDistanceCheckingState::step(DDDTask *task,
                                                 Context *context)
{
  if (this->timer.isFinished())
  {
    context->setState(GlobalState::Inside);
    task->switchState(&task->idleState);
    return;
  }
  auto distance = task->droneDistanceSensor->getDistanceMm();
  task->sendDistance(distance);
  if (distance > task->insideDistanceMm)
  {
    task->switchState(&task->landingReadingState);
  }
}
