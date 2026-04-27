#include "dpd.hpp"

#include "blocking.hpp"

DPDTask::DPDTask(PresenceSensor *dronePresenceSensor,
                 MessageService *messageService)
    : Task<DPDTask>(new IdleState()),
      dronePresenceSensor(dronePresenceSensor),
      messageService(messageService) {}

void DPDTask::IdleState::step(DPDTask *task, Context *context)
{
  blockOnAlarm<DPDTask, IdleState>(task, context);

  switch (context->getState())
  {
  case GlobalState::Outside:
    if (task->messageService->messageAvailable() &&
        task->messageService->getMessage()->getType() ==
            MessageType::REQUEST_LANDING)
    {
      task->switchState(new ReadingState());
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
  task->switchState(new IdleState());
}
