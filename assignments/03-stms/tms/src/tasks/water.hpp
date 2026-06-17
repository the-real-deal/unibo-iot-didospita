#pragma once

#include "task.hpp"
#include "kernel/events.hpp"
#include "core/network.hpp"
#include "core/mqtt.hpp"
#include "devices/led.hpp"
#include "devices/sonar.hpp"

#ifndef WATER_MONITORING_MAX_CAPACITY
#define WATER_MONITORING_MAX_CAPACITY 400
#endif

class WaterMonitoringTask : public EventActor
{
private:
    MQTTClient *mqtt;
    Sonar *sonar;
    const char* waterLevelTopic;

    class NetworkObserver : public TaskEventObserver<WaterMonitoringTask, NetworkState>
    {
    public:
        NetworkObserver(WaterMonitoringTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(NetworkState state) override;
    };
    NetworkObserver networkObserver;

    class SonarObserver : public TaskEventObserver<WaterMonitoringTask, SonarEvent>
    {
    public:
        SonarObserver(WaterMonitoringTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(SonarEvent event) override;
    };
    SonarObserver sonarObserver;

public:
    WaterMonitoringTask(MQTTClient *mqtt,
                        Sonar *sonar,
                        const char* waterLevelTopic,
                        EventFamily sonarEventFamily,
                        EventFamily networkStateEventFamily);
    void begin(EventsManager *eventsManager) override;
};