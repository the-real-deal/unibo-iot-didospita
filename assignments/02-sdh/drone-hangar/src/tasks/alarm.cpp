#include "alarm.hpp"

#include "std/enum.hpp"

AlarmTask::AlarmTask(TemperatureSensor *hangarTempSensor,
                     Indicator *alarmIndicator, GlobalState initialState,
                     float prealarmTemp, uint64_t prealarmTimeMillis,
                     float alarmTemp, uint64_t alarmTimeMillis)
    : Task<AlarmTask>(&this->idleState),
      hangarTempSensor(hangarTempSensor),
      alarmIndicator(alarmIndicator),
      prevState(initialState),
      prealarmTemp(prealarmTemp),
      prealarmTimeMillis(prealarmTimeMillis),
      alarmTemp(alarmTemp),
      alarmTimeMillis(alarmTimeMillis),
      idleState(),
      blockIdleState(&this->idleState),
      prealarmReadingState(),
      prealarmTempCheckingState(this),
      waitOperationsState(),
      alarmReadingState(),
      alarmTempCheckingState(this) {}

void AlarmTask::IdleState::step(AlarmTask *task, Context *context)
{
  blockOnAlarm(task, context, &task->blockIdleState);

  if (task->alarmIndicator->isOn())
  {
    task->alarmIndicator->turnOff();
  }

  switch (context->getState())
  {
  case GlobalState::Inside:
  case GlobalState::Takeoff:
  case GlobalState::Landing:
    task->switchState(&task->prealarmReadingState);
    break;
  default:
    break;
  }
}

void AlarmTask::PrealarmReadingState::step(AlarmTask *task, Context *context)
{
  switch (context->getState())
  {
  case GlobalState::Inside:
  case GlobalState::Takeoff:
  case GlobalState::Landing:
    if (task->hangarTempSensor->getTemperature() >= task->prealarmTemp)
    {
      task->switchState(&task->prealarmTempCheckingState);
    }
    break;
  default:
    task->switchState(&task->idleState);
    break;
  }
}

AlarmTask::PrealarmTempCheckingState::PrealarmTempCheckingState(AlarmTask *task)
    : timer(task->prealarmTimeMillis) {}

void AlarmTask::PrealarmTempCheckingState::setup(AlarmTask *task)
{
  this->timer.start();
}

void AlarmTask::PrealarmTempCheckingState::step(AlarmTask *task,
                                                Context *context)
{
  if (this->timer.isFinished())
  {
    task->switchState(&task->waitOperationsState);
  }
  else if (task->hangarTempSensor->getTemperature() < task->prealarmTemp)
  {
    task->switchState(&task->prealarmReadingState);
  }
}

void AlarmTask::WaitOperationsState::step(AlarmTask *task, Context *context)
{
  GlobalState state = context->getState();
  switch (state)
  {
  case GlobalState::Takeoff:
  case GlobalState::Landing:
    break;
  default:
    task->prevState = state;
    context->setState(GlobalState::Prealarm);
    task->switchState(&task->alarmReadingState);
    break;
  }
}

void AlarmTask::AlarmReadingState::step(AlarmTask *task, Context *context)
{
  float temp = task->hangarTempSensor->getTemperature();
  if (temp >= task->alarmTemp)
  {
    task->switchState(&task->alarmTempCheckingState);
  }
  else if (temp < task->prealarmTemp)
  {
    context->setState(task->prevState);
    task->switchState(&task->prealarmReadingState);
  }
}

AlarmTask::AlarmTempCheckingState::AlarmTempCheckingState(AlarmTask *task)
    : timer(task->alarmTimeMillis) {}

void AlarmTask::AlarmTempCheckingState::setup(AlarmTask *task)
{
  this->timer.start();
}

void AlarmTask::AlarmTempCheckingState::step(AlarmTask *task,
                                             Context *context)
{
  if (this->timer.isFinished())
  {
    task->alarmIndicator->turnOn();

    context->setState(GlobalState::Alarm);
    task->switchState(&task->idleState);
  }
  else if (task->hangarTempSensor->getTemperature() < task->alarmTemp)
  {
    task->switchState(&task->alarmReadingState);
  }
}
