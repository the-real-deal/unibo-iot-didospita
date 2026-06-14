#pragma once

#include "task.hpp"
#include "kernel/events.hpp"
#include "core/network.hpp"
#include "core/mqtt.hpp"
#include "devices/led.hpp"

class NetworkTask : public EventActor
{
private:
    MQTTClient *mqtt;
    Led *onlineLed;
    Led *offlineLed;

    class NetworkObserver : public TaskEventObserver<NetworkTask, NetworkState>
    {
    public:
        NetworkObserver(NetworkTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(NetworkState state) override;
    };
    NetworkObserver networkObserver;

public:
    NetworkTask(MQTTClient *mqtt,
                Led *onlineLed,
                Led *offlineLed,
                EventFamily networkStateEventFamily);
    void begin(EventsManager *eventsManager) override;
};