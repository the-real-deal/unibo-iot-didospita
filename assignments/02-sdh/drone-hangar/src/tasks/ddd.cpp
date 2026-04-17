#include "ddd.hpp"

#include "blocking.hpp"

DDDTask::DDDTask(DistanceSensor *droneDistanceSensor,
                 MessageService *messageService, float outsideDistance,
                 uint64_t outsideTimeMillis, float insideDistance,
                 uint64_t insideTimeMillis)
    : Task<DDDTask>(new IdleState()),
      droneDistanceSensor(droneDistanceSensor),
      messageService(messageService),
      outsideDistanceMm(outsideDistance),
      outsideTimeMillis(outsideTimeMillis),
      insideDistanceMm(insideDistance),
      insideTimeMillis(insideTimeMillis) {}

void DDDTask::sendDistance(float distance)
{
  this->messageService->send(Message(MessageType::DISTANCE, String(distance)));
}

void DDDTask::IdleState::step(DDDTask *task, Context *context)
{
  blockOnAlarm<DDDTask, IdleState>(task, context);

  switch (context->getState())
  {
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

void DDDTask::TakeoffReadingState::step(DDDTask *task, Context *context)
{
  auto distance = task->droneDistanceSensor->getDistanceMm();
  Serial.print(F("TAKEOFF: "));
  Serial.println(distance);
  Serial.flush();
  if (distance >= task->outsideDistanceMm)
  {
    task->switchState(new TakeoffDistanceCheckingState(task));
  }
}

DDDTask::TakeoffDistanceCheckingState::TakeoffDistanceCheckingState(
    DDDTask *task)
    : timer(task->outsideTimeMillis) {}

void DDDTask::TakeoffDistanceCheckingState::step(DDDTask *task,
                                                 Context *context)
{
  Serial.print(F("Elapsed: "));
  Serial.println((uint32_t)this->timer.getElapsedMillis());
  Serial.flush();
  if (this->timer.isFinished())
  {
    context->setState(GlobalState::Outside);
    task->switchState(new IdleState());
    return;
  }

  auto distance = task->droneDistanceSensor->getDistanceMm();
  if (distance < task->outsideDistanceMm)
  {
    task->switchState(new TakeoffReadingState());
  }
}

void DDDTask::LandingReadingState::step(DDDTask *task, Context *context)
{
  auto distance = task->droneDistanceSensor->getDistanceMm();
  task->sendDistance(distance);
  if (distance <= task->insideDistanceMm)
  {
    task->switchState(new LandingDistanceCheckingState(task));
  }
}

DDDTask::LandingDistanceCheckingState::LandingDistanceCheckingState(
    DDDTask *task)
    : timer(task->insideTimeMillis) {}

void DDDTask::LandingDistanceCheckingState::step(DDDTask *task,
                                                 Context *context)
{
  if (this->timer.isFinished())
  {
    context->setState(GlobalState::Inside);
    task->switchState(new IdleState());
    return;
  }
  auto distance = task->droneDistanceSensor->getDistanceMm();
  task->sendDistance(distance);
  if (distance > task->insideDistanceMm)
  {
    task->switchState(new LandingReadingState());
  }
}
