#pragma once

#include "core/tasks.hpp"
#include "io/message.hpp"
#include "io/presence.hpp"

class DPDTask : public Task<DPDTask> {
private:
  PresenceSensor *dronePresenceSensor;
  MessageService *messageService;

  class IdleState : public TaskState<DPDTask> {
  public:
    void step(DPDTask *task, Context *context) override;
  };

  class ReadingState : public TaskState<DPDTask> {
  public:
    void step(DPDTask *task, Context *context) override;
  };

public:
  DPDTask(PresenceSensor *dronePresenceSensor, MessageService *messageService);
};
