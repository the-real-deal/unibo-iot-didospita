#include <Arduino.h>
#include <PubSubClient.h>

#include "config.h"

#include "kernel/events.hpp"
#include "devices/wifi.hpp"
#include "std/enum.hpp"

enum class Events : EventFamily
{
  Wifi,
};

EventFamily family(Events event)
{
  return static_cast<EventFamily>(event);
}

EventsManager eventsManager;
WifiManager wifiManager(WIFI_SSID, WIFI_PASSWD, family(Events::Wifi));

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.println(F("setup() started"));

  wifiManager.begin(&eventsManager);

  Serial.println(F("setup() finished"));
}

void loop()
{
  eventsManager.handleEvents();
  delay(LOOP_DELAY_MS);
}