#include "control.hpp"

#include "config.h"

#include "std/percentage.hpp"

void ControlTask::SystemStateObserver::onEvent(SystemState state)
{
    switch (state)
    {
    case SystemState::Unconnected:
        this->task->potentiometerObserver.disableEvents();
        this->task->serialObserver.disableEvents();
        break;
    case SystemState::Automatic:
        this->task->potentiometerObserver.disableEvents();
        this->task->serialObserver.enableEvents();
        break;
    case SystemState::Manual:
        this->task->potentiometerObserver.enableEvents();
        this->task->serialObserver.enableEvents();
        break;
    }
}

void ControlTask::PotentiometerObserver::onEvent(PotentiometerEvent event)
{
    float percentage = event.value;
    int angle = fromPercentage(percentage, POT_SERVO_MAX_ANGLE);
    this->task->servo->setAngle(angle);
    this->task->serialManager->sendDoorOpening(percentage);
}

void ControlTask::SerialObserver::onEvent(SerialMessage message)
{
    if (message.type != SerialMessageType::Door)
    {
        return;
    }

    double percentage = String(message.data).toDouble();
    int angle = fromPercentage(percentage, POT_SERVO_MAX_ANGLE);
    this->task->servo->setAngle(angle);
    this->task->serialManager->sendDoorOpening(percentage);
}

ControlTask::ControlTask(ServoMotor *servo,
                         SerialManager *serialManager,
                         EventFamily systemStateEventFamily,
                         EventFamily potEventFamily,
                         EventFamily serialEventFamily)
    : servo(servo),
      serialManager(serialManager),
      systemStateObserver(this, systemStateEventFamily),
      potentiometerObserver(this, potEventFamily),
      serialObserver(this, serialEventFamily) {}

void ControlTask::begin(EventsManager *eventsManager)
{
    this->systemStateObserver.begin(eventsManager);
    this->potentiometerObserver.begin(eventsManager);
    this->serialObserver.begin(eventsManager);
}
