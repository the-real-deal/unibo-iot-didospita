#include "dpd.hpp"

DPDTask::IdleState DPDTask::IDLE;
BlockedTaskState<DPDTask> DPDTask::BLOCKED_IDLE(&DPDTask::IDLE);
DPDTask::ReadingState DPDTask::READING;

DPDTask::DPDTask(PresenceSensor *dronePresenceSensor,
                 MessageService *messageService)
    : Task<DPDTask>(&DPDTask::IDLE),
      dronePresenceSensor(dronePresenceSensor),
      messageService(messageService) {}

void DPDTask::IdleState::step(DPDTask *task, Context *context)
{
  blockOnAlarm(task, context, &DPDTask::BLOCKED_IDLE);

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

void DPDTask::ReadingState::step(DPDTask *task, Context *context)
{
  bool dronePresent = task->dronePresenceSensor->isPresent();
  context->setState(dronePresent ? GlobalState::Landing : GlobalState::Outside);
  task->switchState(&DPDTask::IDLE);
}
