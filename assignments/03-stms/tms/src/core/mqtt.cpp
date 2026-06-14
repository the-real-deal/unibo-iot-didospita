#include "mqtt.hpp"

MQTTClient::MQTTClient(WiFiClient &wifiClient,
                       MQTTBrokerEndpoint broker,
                       const char *baseTopic,
                       EventFamily mqttEventFamily)
    : SyncEventSource(mqttEventFamily),
      client(wifiClient),
      broker(broker),
      baseTopic(baseTopic) {}

String MQTTClient::generateClientId()
{
    String id = String(this->baseTopic);
    id.replace(F("/"), F("-"));
    id += '-' + String(random(0xffff), HEX);
    return id;
}

String MQTTClient::generateTopic(const char *topic)
{
    String topicString = String(this->baseTopic);
    topicString.concat(F("/"));
    topicString.concat(topic);
    return topicString;
}

void MQTTClient::callback(char *topic, byte *payload, unsigned int length)
{
    this->generateEvent({.topic = topic, .payload = payload, .length = length});
}

void MQTTClient::begin(EventsManager *eventsManager)
{
    EventSource<MQTTMessage>::begin(eventsManager);
    client.setServer(this->broker.left, this->broker.right);
    this->client.setCallback([this](char *topic, byte *payload, unsigned int length)
                             { this->callback(topic, payload, length); });
}

void MQTTClient::generateEvents()
{
    this->client.loop();
}

void MQTTClient::connect()
{
    // client id generation is expensive, stop if already connected
    bool connected = this->client.connected();
    if (connected)
    {
        return;
    }

    String id = this->generateClientId();
    while (!connected)
    {
        bool connected = this->client.connect(id.c_str());
        if (connected)
        {
            return;
        }
        else
        {
            delay(MQTT_CONNECT_RETRY_PERIOD_MS);
        }
    }
}

bool MQTTClient::subscribe(const char *topic)
{
    String fullTopic = this->generateTopic(topic);
    return this->client.subscribe(fullTopic.c_str());
}

bool MQTTClient::publish(const char *topic, const char *message)
{
    String fullTopic = this->generateTopic(topic);
    return this->client.publish(fullTopic.c_str(), message);
}
