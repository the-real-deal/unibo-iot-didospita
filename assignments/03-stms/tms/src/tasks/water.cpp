#include "water.hpp"

void WaterMonitoringTask::NetworkObserver::onEvent(NetworkState state)
{
    switch (state)
    {
    case NetworkState::Connected:
        this->task->sonar->enableEvents();
        break;
    case NetworkState::Disconnected:
        this->task->sonar->disableEvents();
        break;
    }
}

void WaterMonitoringTask::SonarObserver::onEvent(SonarEvent event)
{
    String distanceString = String(event.distanceMm);
    this->task->mqtt->publish(this->task->waterLevelTopic, distanceString.c_str());
}

WaterMonitoringTask::WaterMonitoringTask(MQTTClient *mqtt,
                                         Sonar *sonar,
                                         const char *waterLevelTopic,
                                         EventFamily sonarEventFamily,
                                         EventFamily networkStateEventFamily)
    : mqtt(mqtt),
      sonar(sonar),
      waterLevelTopic(waterLevelTopic),
      sonarObserver(this, sonarEventFamily),
      networkObserver(this, networkStateEventFamily) {}

void WaterMonitoringTask::begin(EventsManager *eventsManager)
{
    this->sonarObserver.begin(eventsManager);
    this->networkObserver.begin(eventsManager);
}