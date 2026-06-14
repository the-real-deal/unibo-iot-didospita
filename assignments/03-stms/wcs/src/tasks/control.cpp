#include "control.hpp"

#include "config.h"

void ControlTask::SystemStateObserver::onEvent(SystemState state)
{
    switch (state)
    {
    case SystemState::Automatic:
    case SystemState::Unconnected:
        this->task->potentiometerObserver.disableEvents();
        this->task->serialObserver.enableEvents();
        break;
    case SystemState::Manual:
        this->task->potentiometerObserver.enableEvents();
        this->task->serialObserver.disableEvents();
        break;
    }
}

void ControlTask::PotentiometerObserver::onEvent(PotentiometerEvent event)
{
    int angle = event.value * POT_SERVO_MAX_ANGLE;
    this->task->servo->setAngle(angle);
}

void ControlTask::SerialObserver::onEvent(SerialMessage message)
{
    if (message.type != SerialMessageType::Angle)
    {
        return;
    }

    int angle = (int)String(message.data).toInt();
    this->task->servo->setAngle(angle);
}

ControlTask::ControlTask(ServoMotor *servo,
                         EventFamily systemStateEventFamily,
                         EventFamily potEventFamily,
                         EventFamily serialEventFamily)
    : servo(servo),
      systemStateObserver(this, systemStateEventFamily),
      potentiometerObserver(this, potEventFamily),
      serialObserver(this, serialEventFamily) {}

void ControlTask::begin(EventsManager *eventsManager)
{
    this->systemStateObserver.begin(eventsManager);
    this->potentiometerObserver.begin(eventsManager);
    this->serialObserver.begin(eventsManager);
}
