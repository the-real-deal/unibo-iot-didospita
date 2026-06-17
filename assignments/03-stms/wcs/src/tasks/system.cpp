#include "system.hpp"

#include "std/enum.hpp"

bool SystemStateTask::generateStateEvent(SystemState state)
{
    this->state = state;
    return this->generateEvent(state);
}

void SystemStateTask::switchState(SystemState state)
{
    Serial.print(F("STATE: "));
    Serial.print(enumToString(state, SYSTEM_STATE_STRINGS));
    Serial.print(F("/"));
    Serial.println(enumToString(this->state, SYSTEM_STATE_STRINGS));
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
    Serial.print(F("SERIAL STATE: "));
    Serial.println(message.data);
    if (message.type != SerialMessageType::State)
    {
        return;
    }

    SystemState state = enumFromString<SystemState>(message.data, SYSTEM_STATE_STRINGS);
    switch (state)
    {
    case SystemState::Unconnected:
        this->task->buttonObserver.disableEvents();
        break;
    default:
        this->task->buttonObserver.enableEvents();
        break;
    }
    this->task->switchState(state);
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
    this->buttonObserver.begin(eventsManager);
    this->serialObserver.begin(eventsManager);
    this->generateStateEvent(this->state);
}