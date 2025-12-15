#include "ddd.hpp"
#include "blocking.hpp"

DDDTask::DDDTask(DistanceSensor *distanceSensor, float outsideDistance,
                 uint64_t outsideTimeMillis, float insideDistance,
                 uint64_t insideTimeMillis)
    : Task<DDDTask>(new DDDTask::IdleState()), distanceSensor(distanceSensor),
      outsideDistance(outsideDistance), outsideTimeMillis(outsideTimeMillis),
      insideDistance(insideDistance), insideTimeMillis(insideTimeMillis) {}

void DDDTask::IdleState::step(DDDTask *task, SchedulerContext *context) {
  blockOnAlarm(task, context, new DDDTask::IdleState());

  switch (context->getState()) {
  case GlobalState::Takeoff:
    task->switchTo(new DDDTask::TakeoffReadingState());
    break;
  case GlobalState::Landing:
    task->switchTo(new DDDTask::LandingReadingState());
    break;
  default:
    break;
  }
}

void DDDTask::TakeoffReadingState::step(DDDTask *task,
                                        SchedulerContext *context) {
  if (task->distanceSensor->isTooNear() ||
      task->distanceSensor->getDistance() < task->outsideDistance) {
    return;
  }
  task->switchTo(new TakeoffMaintainDistanceState(task));
}

DDDTask::TakeoffMaintainDistanceState::TakeoffMaintainDistanceState(
    DDDTask *task)
    : timer(task->outsideTimeMillis) {}

void DDDTask::TakeoffMaintainDistanceState::step(DDDTask *task,
                                                 SchedulerContext *context) {
  if (this->timer.isFinished()) {
    context->setState(GlobalState::Outside);
    task->switchTo(new DDDTask::IdleState());
  } else if (task->distanceSensor->isTooNear() ||
             task->distanceSensor->getDistance() < task->outsideDistance) {
    task->switchTo(new DDDTask::TakeoffReadingState());
  }
}

void DDDTask::LandingReadingState::step(DDDTask *task,
                                        SchedulerContext *context) {
  if (task->distanceSensor->isTooFar() ||
      task->distanceSensor->getDistance() > task->insideDistance) {
    return;
  }
  task->switchTo(new LandingMaintainDistanceState(task));
}

DDDTask::LandingMaintainDistanceState::LandingMaintainDistanceState(
    DDDTask *task)
    : timer(task->insideTimeMillis) {}

void DDDTask::LandingMaintainDistanceState::step(DDDTask *task,
                                                 SchedulerContext *context) {
  if (this->timer.isFinished()) {
    context->setState(GlobalState::Inside);
    task->switchTo(new DDDTask::IdleState());
  } else if (task->distanceSensor->isTooFar() ||
             task->distanceSensor->getDistance() > task->insideDistance) {
    task->switchTo(new DDDTask::LandingReadingState());
  }
}
