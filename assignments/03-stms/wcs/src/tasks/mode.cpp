#include "mode.hpp"

#include "std/enum.hpp"

bool OperationModeTask::generateModeEvent(OperationMode mode)
{
    OperationModeEvent event = {.mode = mode, .prev = this->mode};
    this->mode = mode;
    bool ok = this->generateEvent(event);
    return ok;
}

void OperationModeTask::toggleMode()
{
    OperationMode mode;
    switch (this->mode)
    {
    case OperationMode::Manual:
        mode = OperationMode::Automatic;
        break;
    case OperationMode::Automatic:
        mode = OperationMode::Manual;
        break;
    }
    this->generateModeEvent(mode);
}

void OperationModeTask::switchMode(OperationMode mode)
{
    if (this->mode == mode)
    {
        return;
    }
    this->generateModeEvent(mode);
}

void OperationModeTask::ButtonObserver::onEvent(ButtonEvent event)
{
    if (event == ButtonEvent::Press)
    {
        this->task->toggleMode();
    }
}

void OperationModeTask::SerialObserver::onEvent(SerialMessage message)
{
    if (message.type != SerialMessageType::Mode)
    {
        return;
    }

    OperationMode mode = enumFromString<OperationMode>(message.data, OPERATION_MODE_STRINGS);
    this->task->switchMode(mode);
}

OperationModeTask::OperationModeTask(OperationMode initialMode,
                                     EventFamily operationsModeEventFamily,
                                     EventFamily buttonEventFamily,
                                     EventFamily serialEventFamily)
    : EventSource(operationsModeEventFamily),
      mode(initialMode),
      buttonObserver(this, buttonEventFamily),
      serialObserver(this, serialEventFamily) {}

void OperationModeTask::OperationModeTask::begin(EventsManager *eventsManager)
{
    EventSource<OperationModeEvent>::begin(eventsManager);
    eventsManager->registerObserver(&this->buttonObserver);
    eventsManager->registerObserver(&this->serialObserver);
    this->generateModeEvent(this->mode);
}