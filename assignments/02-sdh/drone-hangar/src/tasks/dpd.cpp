#include "dpd.hpp"

#include "config.h"

DPDTask::IdleState DPDTask::IDLE;
BlockedTaskState<DPDTask> DPDTask::BLOCKED_IDLE(&DPDTask::IDLE);
DPDTask::ReadingState DPDTask::READING;

DPDTask::DPDTask(PresenceSensor *dronePresenceSensor,
                 MessageService *messageService)
    : Task<DPDTask>(&DPDTask::IDLE),
      dronePresenceSensor(dronePresenceSensor),
      messageService(messageService),
      timer() {}

void DPDTask::IdleState::step(DPDTask *task, Context *context)
{
  if (context->getState() == GlobalState::Alarm)
  {
    task->switchState(&DPDTask::BLOCKED_IDLE);
    return;
  }

  switch (context->getState())
  {
  case GlobalState::Outside:
    if (task->messageService->messageAvailable() &&
        task->messageService->getMessage()->getType() ==
            MessageType::REQUEST_LANDING)
    {
      task->switchState(&DPDTask::READING);
    }
    break;
  default:
    break;
  }
}

void DPDTask::ReadingState::setup(DPDTask *task)
{
  task->timer = Timer(DPD_REQUEST_LANDING_TIMEOUT_MS);
  task->timer.start();
}

void DPDTask::ReadingState::step(DPDTask *task, Context *context)
{
  bool dronePresent = task->dronePresenceSensor->isPresent();
  if (dronePresent)
  {
    context->setState(GlobalState::Landing);
  }
  else if (task->timer.isFinished())
  {
    context->setState(GlobalState::Outside);
  }
  else
  {
    return;
  }
  task->switchState(&DPDTask::IDLE);
}
