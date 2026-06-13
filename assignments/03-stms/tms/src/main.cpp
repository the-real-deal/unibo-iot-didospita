#include <Arduino.h>

#include "config.h"

#include "kernel/events.hpp"
#include "devices/wifi.hpp"
#include "core/mqtt.hpp"
#include "std/enum.hpp"

enum class Events : EventFamily
{
  NetworkState,
  MQTT,
};

EventFamily family(Events event)
{
  return static_cast<EventFamily>(event);
}

EventsManager eventsManager;
WifiManager wifiManager(WIFI_SSID, WIFI_PASSWD, family(Events::NetworkState));
MQTTClient mqtt(wifiManager.getClient(),
                MQTTBrokerEndpoint(MQTT_BROKER_HOST, MQTT_BROKER_PORT),
                MQTT_BASE_TOPIC,
                family(Events::MQTT));

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.println(F("setup() started"));

  randomSeed(micros());
  wifiManager.begin(&eventsManager);

  Serial.println(F("setup() finished"));
}

void loop()
{
  eventsManager.handleEvents();
  delay(LOOP_DELAY_MS);
}