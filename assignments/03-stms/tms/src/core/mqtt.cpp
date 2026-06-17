#include "mqtt.hpp"

MQTTClient::MQTTClient(WiFiClient &wifiClient,
                       MQTTBrokerEndpoint broker,
                       const char *baseTopic,
                       EventFamily mqttEventFamily)
    : SyncEventSource(mqttEventFamily),
      client(wifiClient),
      broker(broker),
      baseTopic(baseTopic)
{
    this->disableEvents(); // enabled only when connected to prevent network errors
}

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
    Serial.flush();
    this->client.loop();
}

void MQTTClient::connect()
{
    Serial.flush();
    // client id generation is expensive, don't do it if already connected
    bool connected = this->client.connected();
    if (!connected)
    {
        String id = this->generateClientId();
        while (!connected)
        {
            bool connected = this->client.connect(id.c_str());
            if (connected)
            {
                break;
            }
            else
            {
                delay(MQTT_CONNECT_RETRY_PERIOD_MS);
            }
        }
    }
    this->enableEvents();
}

void MQTTClient::disconnect()
{
    Serial.flush();
    this->disableEvents();
    this->client.disconnect();
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
