#include "wifi.hpp"

WifiManager::WifiManager(const char *ssid,
                         const char *passwd,
                         EventFamily networkEventFamily)
    : EventSource(networkEventFamily),
      ssid(ssid), passwd(passwd),
      state(NetworkState::Disconnected),
      client(),
      eventsAttached(false),
      onConnectEvent(),
      onDisconnectEvent() {}

WifiManager::~WifiManager()
{
  if (this->eventsAttached)
  {
    WiFi.removeEvent(this->onConnectEvent);
    WiFi.removeEvent(this->onDisconnectEvent);
  }
}

bool WifiManager::generateStateEvent()
{
  return this->generateEvent(this->state);
}

void WifiManager::switchState(NetworkState state)
{
  if (this->state == state)
  {
    return;
  }
  this->state = state;
  this->generateStateEvent();
}

void WifiManager::onConnect(arduino_event_id_t id, arduino_event_info_t info)
{
  Serial.println(F("WIFI CONNECTED"));
  Serial.flush();
  this->switchState(NetworkState::Connected);
}

void WifiManager::onDisconnect(arduino_event_id_t id, arduino_event_info_t info)
{
  Serial.println(F("WIFI DISCONNECTED"));
  Serial.flush();
  this->switchState(NetworkState::Disconnected);
}

void WifiManager::begin(EventsManager *eventsManager)
{
  EventSource<NetworkState>::begin(eventsManager);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  this->onConnectEvent = WiFi.onEvent([this](arduino_event_id_t id, arduino_event_info_t info)
                                      { this->onConnect(id, info); }, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  this->onDisconnectEvent = WiFi.onEvent([this](arduino_event_id_t id, arduino_event_info_t info)
                                         { this->onDisconnect(id, info); }, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  this->eventsAttached = true;
  WiFi.begin(this->ssid, this->passwd);
  this->generateStateEvent();
}

WiFiClient &WifiManager::getClient()
{
  return this->client;
}
