#pragma once

#include <WiFi.h>
#include <PubSubClient.h>

#include "kernel/events.hpp"
#include "std/pair.hpp"

#ifndef MQTT_CONNECT_RETRY_PERIOD_MS
#define MQTT_CONNECT_RETRY_PERIOD_MS 5000
#endif

struct MQTTMessage
{
    char *topic;
    byte *payload;
    unsigned int length;
};

using MQTTBrokerEndpoint = Pair<const char *, uint16_t>;

class MQTTClient : public SyncEventSource<MQTTMessage>
{
private:
    PubSubClient client;
    MQTTBrokerEndpoint broker;
    const char *baseTopic;

    String generateClientId();
    String generateTopic(const char* topic);
    void callback(char *topic, byte *payload, unsigned int length);

public:
    MQTTClient(WiFiClient &wifiClient, MQTTBrokerEndpoint broker,
               const char *baseTopic,
               EventFamily mqttEventFamily);
    void begin(EventsManager *eventsManager) override;
    void checkEvents() override;
    void connect();
    bool subscribe(const char* topic);
    bool publish(const char* topic, const char* message);
};
