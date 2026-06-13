#include "system.hpp"

#include "std/enum.hpp"

bool SystemStateTask::generateStateEvent(SystemState state)
{
    this->state = state;
    return this->generateEvent(state);
}

void SystemStateTask::switchState(SystemState state)
{
    if (this->state == state)
    {
        return;
    }
    this->generateStateEvent(state);
}

void SystemStateTask::ButtonObserver::onEvent(ButtonEvent event)
{
    if (event != ButtonEvent::Press)
    {
        return;
    }

    SystemState state;
    switch (this->task->state)
    {
    case SystemState::Manual:
        state = SystemState::Automatic;
        break;
    case SystemState::Automatic:
        state = SystemState::Manual;
        break;
    default:
        break;
    }
    this->task->switchState(state);
}

void SystemStateTask::SerialObserver::onEvent(SerialMessage message)
{
    if (message.type != SerialMessageType::State)
    {
        return;
    }

    SystemState state = enumFromString<SystemState>(message.data, SYSTEM_STATE_STRINGS);
    switch (state)
    {
    case SystemState::Unconnected:
        this->task->buttonObserver.disable();
        break;
    default:
        this->task->buttonObserver.enable();
        this->task->switchState(state);
        break;
    }
}

SystemStateTask::SystemStateTask(SystemState initialState,
                                   EventFamily systemStateEventFamily,
                                   EventFamily buttonEventFamily,
                                   EventFamily serialEventFamily)
    : EventSource(systemStateEventFamily),
      state(initialState),
      buttonObserver(this, buttonEventFamily),
      serialObserver(this, serialEventFamily) {}

void SystemStateTask::SystemStateTask::begin(EventsManager *eventsManager)
{
    EventSource<SystemState>::begin(eventsManager);
    eventsManager->registerObserver(&this->buttonObserver);
    eventsManager->registerObserver(&this->serialObserver);
    this->generateStateEvent(this->state);
}