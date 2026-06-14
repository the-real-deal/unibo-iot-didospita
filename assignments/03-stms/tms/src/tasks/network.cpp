#include "network.hpp"

void NetworkTask::NetworkObserver::onEvent(NetworkState state)
{
    switch (state)
    {
    case NetworkState::Connected:
        this->task->mqtt->connect();
        this->task->mqtt->enable();
        this->task->onlineLed->turnOn();
        this->task->offlineLed->turnOff();
        break;
    case NetworkState::Disconnected:
        this->task->mqtt->disable();
        this->task->onlineLed->turnOff();
        this->task->offlineLed->turnOn();
        break;
    }
}

NetworkTask::NetworkTask(MQTTClient *mqtt,
                         Led *onlineLed,
                         Led *offlineLed,
                         EventFamily networkStateEventFamily)
    : mqtt(mqtt),
      onlineLed(onlineLed),
      offlineLed(offlineLed),
      networkObserver(this, networkStateEventFamily) {}

void NetworkTask::begin(EventsManager *eventsManager)
{
    this->networkObserver.begin(eventsManager);
}