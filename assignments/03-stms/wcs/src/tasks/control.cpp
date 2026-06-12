#include "control.hpp"

#include "config.h"

void ControlTask::SystemStateObserver::onEvent(SystemStatusChangeEvent event)
{
    switch (event.status)
    {
    case SystemStatus::Automatic:
    case SystemStatus::Unconnected:
        this->task->potentiometerObserver.disable();
        break;
    case SystemStatus::Manual:
        this->task->potentiometerObserver.enable();
        break;
    }
}

void ControlTask::PotentiometerObserver::onEvent(PotentiometerEvent event)
{
    int angle = event.value * POT_SERVO_MAX_ANGLE;
    this->task->servo->setAngle(angle);
}

ControlTask::ControlTask(ServoMotor *servo,
                                     EventFamily statusChangeEventFamily,
                                     EventFamily potEventFamily)
    : servo(servo),
      systemStateObserver(this, statusChangeEventFamily),
      potentiometerObserver(this, potEventFamily) {}

void ControlTask::begin(EventsManager *eventsManager)
{
    eventsManager->registerObserver(&this->systemStateObserver);
    eventsManager->registerObserver(&this->potentiometerObserver);
}
