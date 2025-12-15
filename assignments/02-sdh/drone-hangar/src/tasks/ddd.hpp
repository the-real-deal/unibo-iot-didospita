#pragma once

#include "core/tasks.hpp"
#include "io/distance.hpp"

class DDDTask : public Task<DDDTask> {
private:
  DistanceSensor *droneDistanceSensor;
  float outsideDistance;
  uint64_t outsideTimeMillis;
  float insideDistance;
  uint64_t insideTimeMillis;

  class IdleState : public TaskState<DDDTask> {
  public:
    void step(DDDTask *task, SchedulerContext *context) override;
  };

  class TakeoffReadingState : public TaskState<DDDTask> {
  public:
    void step(DDDTask *task, SchedulerContext *context) override;
  };

  class TakeoffMaintainDistanceState : public TaskState<DDDTask> {
  private:
    Timer timer;

  public:
    TakeoffMaintainDistanceState(DDDTask *task);
    void step(DDDTask *task, SchedulerContext *context) override;
  };

  class LandingReadingState : public TaskState<DDDTask> {
  public:
    void step(DDDTask *task, SchedulerContext *context) override;
  };

  class LandingMaintainDistanceState : public TaskState<DDDTask> {
  private:
    Timer timer;

  public:
    LandingMaintainDistanceState(DDDTask *task);
    void step(DDDTask *task, SchedulerContext *context) override;
  };

public:
  DDDTask(DistanceSensor *droneDistanceSensor, float outsideDistance,
          uint64_t outsideTimeMillis, float insideDistance,
          uint64_t insideTimeMillis);
};
