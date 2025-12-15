#include "ddd.hpp"
#include "blocking.hpp"

DDDTask::DDDTask(DistanceSensor *droneDistanceSensor, float outsideDistance,
                 uint64_t outsideTimeMillis, float insideDistance,
                 uint64_t insideTimeMillis)
    : Task<DDDTask>(new DDDTask::IdleState()),
      droneDistanceSensor(droneDistanceSensor),
      outsideDistance(outsideDistance), outsideTimeMillis(outsideTimeMillis),
      insideDistance(insideDistance), insideTimeMillis(insideTimeMillis) {}

void DDDTask::IdleState::step(DDDTask *task, SchedulerContext *context) {
  blockOnAlarm<DDDTask, DDDTask::IdleState>(task, context);

  switch (context->getState()) {
  case GlobalState::Takeoff:
    task->switchState(new DDDTask::TakeoffReadingState());
    break;
  case GlobalState::Landing:
    task->switchState(new DDDTask::LandingReadingState());
    break;
  default:
    break;
  }
}

void DDDTask::TakeoffReadingState::step(DDDTask *task,
                                        SchedulerContext *context) {
  if (task->droneDistanceSensor->isTooNear() ||
      task->droneDistanceSensor->getDistance() < task->outsideDistance) {
    return;
  }
  task->switchState(new TakeoffMaintainDistanceState(task));
}

DDDTask::TakeoffMaintainDistanceState::TakeoffMaintainDistanceState(
    DDDTask *task)
    : timer(task->outsideTimeMillis) {}

void DDDTask::TakeoffMaintainDistanceState::step(DDDTask *task,
                                                 SchedulerContext *context) {
  if (this->timer.isFinished()) {
    context->setState(GlobalState::Outside);
    task->switchState(new DDDTask::IdleState());
  } else if (task->droneDistanceSensor->isTooNear() ||
             task->droneDistanceSensor->getDistance() < task->outsideDistance) {
    task->switchState(new DDDTask::TakeoffReadingState());
  }
}

void DDDTask::LandingReadingState::step(DDDTask *task,
                                        SchedulerContext *context) {
  if (task->droneDistanceSensor->isTooFar() ||
      task->droneDistanceSensor->getDistance() > task->insideDistance) {
    return;
  }
  task->switchState(new LandingMaintainDistanceState(task));
}

DDDTask::LandingMaintainDistanceState::LandingMaintainDistanceState(
    DDDTask *task)
    : timer(task->insideTimeMillis) {}

void DDDTask::LandingMaintainDistanceState::step(DDDTask *task,
                                                 SchedulerContext *context) {
  if (this->timer.isFinished()) {
    context->setState(GlobalState::Inside);
    task->switchState(new DDDTask::IdleState());
  } else if (task->droneDistanceSensor->isTooFar() ||
             task->droneDistanceSensor->getDistance() > task->insideDistance) {
    task->switchState(new DDDTask::LandingReadingState());
  }
}
