#include <Arduino.h>

#include "config.h"

#include "kernel/events.hpp"
#include "devices/wifi.hpp"
#include "devices/sonar.hpp"
#include "devices/led.hpp"
#include "core/mqtt.hpp"
#include "std/enum.hpp"
#include "tasks/network.hpp"

enum class Events : EventFamily
{
  NetworkState,
  MQTT,
  Sonar,
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
Sonar sonar(SONAR_ECHO_PIN, SONAR_TRIGGER_PIN, family(Events::Sonar));

Led builtinLed(BUILTIN_LED);
Led offlineLed(OFFLINE_LED_PIN);
Led onlineLed(ONLINE_LED_PIN);

NetworkTask networkTask(&mqtt, &onlineLed, &offlineLed, family(Events::NetworkState));

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.println(F("setup() started"));

  randomSeed(micros());
  builtinLed.setup();
  offlineLed.setup();
  onlineLed.setup();

  wifiManager.begin(&eventsManager);
  mqtt.begin(&eventsManager);
  sonar.begin(&eventsManager);

  networkTask.begin(&eventsManager);
  sonar.spawnBackgroundTask("SONAR_EVENTS_TASK", SONAR_EVENTS_PERIOD_MS);
  mqtt.spawnBackgroundTask("MQTT_EVENTS_TASK", MQTT_EVENTS_PERIOD_MS);

  Serial.println(F("setup() finished"));
}

void loop()
{
  builtinLed.toggle();
  eventsManager.handleEvents();
  delay(LOOP_PERIOD_MS);
}