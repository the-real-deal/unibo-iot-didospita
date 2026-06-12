#include "control.hpp"

#include "config.h"

void ControlTask::SystemStateObserver::onEvent(SystemStatusChangeEvent event)
{
    switch (event.status)
    {
    case SystemStatus::Automatic:
    case SystemStatus::Unconnected:
        this->task->potentiometerObserver.disable();
        this->task->serialObserver.enable();
        break;
    case SystemStatus::Manual:
        this->task->potentiometerObserver.enable();
        this->task->serialObserver.disable();
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
                         EventFamily statusChangeEventFamily,
                         EventFamily potEventFamily,
                         EventFamily serialEventFamily)
    : servo(servo),
      systemStateObserver(this, statusChangeEventFamily),
      potentiometerObserver(this, potEventFamily),
      serialObserver(this, serialEventFamily) {}

void ControlTask::begin(EventsManager *eventsManager)
{
    eventsManager->registerObserver(&this->systemStateObserver);
    eventsManager->registerObserver(&this->potentiometerObserver);
    eventsManager->registerObserver(&this->serialObserver);
}
