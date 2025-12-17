#pragma once

#include "core/tasks.hpp"
#include "io/indicator.hpp"
#include "io/message.hpp"
#include "io/temperature.hpp"

class AlarmTask : public Task<AlarmTask> {
 private:
  TemperatureSensor* hangarTempSensor;
  Indicator* alarmIndicator;
  GlobalState prevState;
  float prealarmTemp;
  uint64_t prealarmTimeMillis;
  float alarmTemp;
  uint64_t alarmTimeMillis;

  class IdleState : public TaskState<AlarmTask> {
   public:
    void step(AlarmTask* task, Context* context) override;
  };

  class PrealarmReadingState : public TaskState<AlarmTask> {
   public:
    void step(AlarmTask* task, Context* context) override;
  };

  class PrealarmTempCheckingState : public TaskState<AlarmTask> {
   private:
    Timer timer;

   public:
    PrealarmTempCheckingState(AlarmTask* task);
    void step(AlarmTask* task, Context* context) override;
  };

  class WaitOperationsState : public TaskState<AlarmTask> {
   public:
    void step(AlarmTask* task, Context* context) override;
  };

  class AlarmReadingState : public TaskState<AlarmTask> {
   public:
    void step(AlarmTask* task, Context* context) override;
  };

  class AlarmTempCheckingState : public TaskState<AlarmTask> {
   private:
    Timer timer;

   public:
    AlarmTempCheckingState(AlarmTask* task);
    void step(AlarmTask* task, Context* context) override;
  };

 public:
  AlarmTask(TemperatureSensor* hangarTempSensor, Indicator* alarmIndicator,
            GlobalState initialState, float prealarmTemp,
            uint64_t prealarmTimeMillis, float alarmTemp,
            uint64_t alarmTimeMillis);
};
