#pragma once

#include "core/tasks.hpp"
#include "io/distance.hpp"
#include "io/message.hpp"

class DDDTask : public Task<DDDTask> {
 private:
  DistanceSensor* droneDistanceSensor;
  MessageService* messageService;
  float outsideDistance;
  uint64_t outsideTimeMillis;
  float insideDistance;
  uint64_t insideTimeMillis;

  void sendDistance(float distance);

  class IdleState : public TaskState<DDDTask> {
   public:
    void step(DDDTask* task, Context* context) override;
  };

  class TakeoffReadingState : public TaskState<DDDTask> {
   public:
    void step(DDDTask* task, Context* context) override;
  };

  class TakeoffDistanceCheckingState : public TaskState<DDDTask> {
   private:
    Timer timer;

   public:
    TakeoffDistanceCheckingState(DDDTask* task);
    void step(DDDTask* task, Context* context) override;
  };

  class LandingReadingState : public TaskState<DDDTask> {
   public:
    void step(DDDTask* task, Context* context) override;
  };

  class LandingDistanceCheckingState : public TaskState<DDDTask> {
   private:
    Timer timer;

   public:
    LandingDistanceCheckingState(DDDTask* task);
    void step(DDDTask* task, Context* context) override;
  };

 public:
  DDDTask(DistanceSensor* droneDistanceSensor, MessageService* messageService,
          float outsideDistance, uint64_t outsideTimeMillis,
          float insideDistance, uint64_t insideTimeMillis);
};
