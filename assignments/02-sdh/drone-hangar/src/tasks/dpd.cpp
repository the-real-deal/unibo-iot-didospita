#include "dpd.hpp"
#include "blocking.hpp"

DPDTask::DPDTask(PresenceSensor *dronePresenceSensor)
    : Task<DPDTask>(new DPDTask::IdleState()),
      dronePresenceSensor(dronePresenceSensor) {}

void DPDTask::IdleState::step(DPDTask *task, SchedulerContext *context) {
  blockOnAlarm<DPDTask, DPDTask::IdleState>(task, context);

  switch (context->getState()) {
  case GlobalState::RequestLanding:
    if (context->getPreviousState() == GlobalState::Outside) {
      task->switchState(new DPDTask::ReadingState());
    }
    break;
  default:
    break;
  }
}

void DPDTask::ReadingState::step(DPDTask *task, SchedulerContext *context) {
  context->setState(task->dronePresenceSensor->isPresent()
                        ? GlobalState::Landing
                        : GlobalState::Outside);
  task->switchState(new DPDTask::IdleState());
}
