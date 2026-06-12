#include "manual.hpp"

void ManualControlTask::SystemStateObserver::onEvent(SystemStatusChangeEvent event)
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

void ManualControlTask::PotentiometerObserver::onEvent(PotentiometerEvent event)
{
    int angle = event.value * 90;
    this->task->servo->setAngle(angle);
}

ManualControlTask::ManualControlTask(ServoMotor *servo,
                                     EventFamily statusChangeEventFamily,
                                     EventFamily potEventFamily)
    : servo(servo),
      systemStateObserver(this, statusChangeEventFamily),
      potentiometerObserver(this, potEventFamily) {}

void ManualControlTask::begin(EventsManager *eventsManager)
{
    eventsManager->registerObserver(&this->systemStateObserver);
    eventsManager->registerObserver(&this->potentiometerObserver);
}
