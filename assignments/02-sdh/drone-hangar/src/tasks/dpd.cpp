#include "dpd.hpp"

DPDTask::DPDTask(PresenceSensor *dronePresenceSensor,
                 MessageService *messageService)
    : Task<DPDTask>(&this->idleState),
      dronePresenceSensor(dronePresenceSensor),
      messageService(messageService),
      idleState(),
      blockedIdleState(&this->idleState),
      readingState() {}

void DPDTask::IdleState::step(DPDTask *task, Context *context)
{
  blockOnAlarm(task, context, &task->blockedIdleState);

  switch (context->getState())
  {
  case GlobalState::Outside:
    if (task->messageService->messageAvailable() &&
        task->messageService->getMessage()->getType() ==
            MessageType::REQUEST_LANDING)
    {
      task->switchState(&task->readingState);
    }
    break;
  default:
    break;
  }
}

void DPDTask::ReadingState::step(DPDTask *task, Context *context)
{
  auto dronePresent = task->dronePresenceSensor->isPresent();
  Serial.print(F("DRONE PRESENT: "));
  Serial.println(dronePresent);
  Serial.flush();
  context->setState(dronePresent ? GlobalState::Landing : GlobalState::Outside);
  task->switchState(&task->idleState);
}
