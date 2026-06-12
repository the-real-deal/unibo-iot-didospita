#include "system.hpp"

#include "std/enum.hpp"


bool SystemStatusTask::generateStatusEvent(SystemStatus status)
{
    SystemStatusChangeEvent event = {.status = status, .prev = this->status};
    this->status = status;
    return this->generateEvent(event);
}

void SystemStatusTask::switchStatus(SystemStatus status)
{
    if (this->status == status)
    {
        return;
    }
    this->generateStatusEvent(status);
}

void SystemStatusTask::ButtonObserver::onEvent(ButtonEvent event)
{
    if (event != ButtonEvent::Press)
    {
        return;
    }

    SystemStatus status;
    switch (this->task->status)
    {
    case SystemStatus::Manual:
        status = SystemStatus::Automatic;
        break;
    case SystemStatus::Automatic:
        status = SystemStatus::Manual;
        break;
    default:
        break;
    }
    this->task->switchStatus(status);
}

void SystemStatusTask::SerialObserver::onEvent(SerialMessage message)
{
    if (message.type != SerialMessageType::Status)
    {
        return;
    }

    SystemStatus status = enumFromString<SystemStatus>(message.data, SYSTEM_STATUS_STRINGS);
    this->task->switchStatus(status);
}

SystemStatusTask::SystemStatusTask(SystemStatus initialStatus,
                                     EventFamily operationsModeEventFamily,
                                     EventFamily buttonEventFamily,
                                     EventFamily serialEventFamily)
    : EventSource(operationsModeEventFamily),
      status(initialStatus),
      buttonObserver(this, buttonEventFamily),
      serialObserver(this, serialEventFamily) {}

void SystemStatusTask::SystemStatusTask::begin(EventsManager *eventsManager)
{
    EventSource<SystemStatusChangeEvent>::begin(eventsManager);
    eventsManager->registerObserver(&this->buttonObserver);
    eventsManager->registerObserver(&this->serialObserver);
    this->generateStatusEvent(this->status);
}