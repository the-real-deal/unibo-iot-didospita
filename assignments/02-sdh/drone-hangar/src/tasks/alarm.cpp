#include "alarm.hpp"

#include "std/enum.hpp"

AlarmTask::IdleState AlarmTask::IDLE;
BlockedTaskState<AlarmTask> AlarmTask::BLOCK_IDLE(&AlarmTask::IDLE);
AlarmTask::PrealarmReadingState AlarmTask::PREALARM_READING;
AlarmTask::PrealarmTempCheckingState AlarmTask::PREALARM_TEMP_CHECKING;
AlarmTask::WaitOperationsState AlarmTask::WAIT_OPERATIONS;
AlarmTask::AlarmReadingState AlarmTask::ALARM_READING;
AlarmTask::AlarmTempCheckingState AlarmTask::ALARM_TEMP_CHECKING;

AlarmTask::AlarmTask(TemperatureSensor *hangarTempSensor,
                     Indicator *alarmIndicator, GlobalState initialState,
                     float prealarmTemp, uint64_t prealarmTimeMillis,
                     float alarmTemp, uint64_t alarmTimeMillis)
    : Task<AlarmTask>(&AlarmTask::IDLE),
      hangarTempSensor(hangarTempSensor),
      alarmIndicator(alarmIndicator),
      prevState(initialState),
      prealarmTemp(prealarmTemp),
      prealarmTimeMillis(prealarmTimeMillis),
      alarmTemp(alarmTemp),
      alarmTimeMillis(alarmTimeMillis),
      timer() {}

void AlarmTask::IdleState::step(AlarmTask *task, Context *context)
{
  blockOnAlarm(task, context, &AlarmTask::BLOCK_IDLE);

  if (task->alarmIndicator->isOn())
  {
    task->alarmIndicator->turnOff();
  }

  switch (context->getState())
  {
  case GlobalState::Inside:
  case GlobalState::Takeoff:
  case GlobalState::Landing:
    task->switchState(&AlarmTask::PREALARM_READING);
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
      task->switchState(&AlarmTask::PREALARM_TEMP_CHECKING);
    }
    break;
  default:
    task->switchState(&AlarmTask::IDLE);
    break;
  }
}

void AlarmTask::PrealarmTempCheckingState::setup(AlarmTask *task)
{
  task->timer = Timer(task->prealarmTimeMillis);
  task->timer.start();
}

void AlarmTask::PrealarmTempCheckingState::step(AlarmTask *task,
                                                Context *context)
{
  if (task->timer.isFinished())
  {
    task->switchState(&AlarmTask::WAIT_OPERATIONS);
  }
  else if (task->hangarTempSensor->getTemperature() < task->prealarmTemp)
  {
    task->switchState(&AlarmTask::PREALARM_READING);
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
    task->switchState(&AlarmTask::ALARM_READING);
    break;
  }
}

void AlarmTask::AlarmReadingState::step(AlarmTask *task, Context *context)
{
  float temp = task->hangarTempSensor->getTemperature();
  if (temp >= task->alarmTemp)
  {
    task->switchState(&AlarmTask::ALARM_TEMP_CHECKING);
  }
  else if (temp < task->prealarmTemp)
  {
    context->setState(task->prevState);
    task->switchState(&AlarmTask::PREALARM_READING);
  }
}

void AlarmTask::AlarmTempCheckingState::setup(AlarmTask *task)
{
  task->timer = Timer(task->alarmTimeMillis);
  task->timer.start();
}

void AlarmTask::AlarmTempCheckingState::step(AlarmTask *task,
                                             Context *context)
{
  if (task->timer.isFinished())
  {
    task->alarmIndicator->turnOn();

    context->setState(GlobalState::Alarm);
    task->switchState(&AlarmTask::IDLE);
  }
  else if (task->hangarTempSensor->getTemperature() < task->alarmTemp)
  {
    task->switchState(&AlarmTask::ALARM_READING);
  }
}
