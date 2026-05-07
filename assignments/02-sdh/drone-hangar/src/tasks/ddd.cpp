#include "ddd.hpp"

DDDTask::IdleState DDDTask::IDLE;
BlockedTaskState<DDDTask> DDDTask::BLOCKED_IDLE(&DDDTask::IDLE);
DDDTask::TakeoffReadingState DDDTask::TAKEOFF_READING;
DDDTask::TakeoffDistanceCheckingState DDDTask::TAKEOFF_DISTANCE_CHECKING;
DDDTask::LandingReadingState DDDTask::LANDING_READING;
DDDTask::LandingDistanceCheckingState DDDTask::LANDING_DISTANCE_CHECKING;

DDDTask::DDDTask(DistanceSensor *droneDistanceSensor,
                 MessageService *messageService, float outsideDistance,
                 uint32_t outsideTimeMillis, float insideDistance,
                 uint32_t insideTimeMillis)
    : Task<DDDTask>(&DDDTask::IDLE),
      droneDistanceSensor(droneDistanceSensor),
      messageService(messageService),
      outsideDistanceMm(outsideDistance),
      outsideTimeMillis(outsideTimeMillis),
      insideDistanceMm(insideDistance),
      insideTimeMillis(insideTimeMillis),
      timer() {}

void DDDTask::sendDistance(float distance)
{
  this->messageService->send(Message(MessageType::DISTANCE, String(distance)));
}

void DDDTask::IdleState::step(DDDTask *task, Context *context)
{
  blockOnAlarm(task, context, &DDDTask::BLOCKED_IDLE);

  switch (context->getState())
  {
  case GlobalState::Takeoff:
    task->switchState(&DDDTask::TAKEOFF_READING);
    break;
  case GlobalState::Landing:
    task->switchState(&DDDTask::LANDING_READING);
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
    task->switchState(&DDDTask::TAKEOFF_DISTANCE_CHECKING);
  }
}

void DDDTask::TakeoffDistanceCheckingState::setup(DDDTask *task)
{
  task->timer = Timer(task->outsideTimeMillis);
  task->timer.start();
}

void DDDTask::TakeoffDistanceCheckingState::step(DDDTask *task,
                                                 Context *context)
{
  if (task->timer.isFinished())
  {
    context->setState(GlobalState::Outside);
    task->switchState(&DDDTask::IDLE);
    return;
  }

  auto distance = task->droneDistanceSensor->getDistanceMm();
  if (distance < task->outsideDistanceMm)
  {
    task->switchState(&DDDTask::TAKEOFF_READING);
  }
}

void DDDTask::LandingReadingState::step(DDDTask *task, Context *context)
{
  auto distance = task->droneDistanceSensor->getDistanceMm();
  task->sendDistance(distance);
  if (distance <= task->insideDistanceMm)
  {
    task->switchState(&DDDTask::LANDING_DISTANCE_CHECKING);
  }
}

void DDDTask::LandingDistanceCheckingState::setup(DDDTask *task)
{
  task->timer = Timer(task->insideTimeMillis);
  task->timer.start();
}

void DDDTask::LandingDistanceCheckingState::step(DDDTask *task,
                                                 Context *context)
{
  if (task->timer.isFinished())
  {
    context->setState(GlobalState::Inside);
    task->switchState(&DDDTask::IDLE);
    return;
  }
  auto distance = task->droneDistanceSensor->getDistanceMm();
  task->sendDistance(distance);
  if (distance > task->insideDistanceMm)
  {
    task->switchState(&DDDTask::LANDING_READING);
  }
}
