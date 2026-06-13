#include "wifi.hpp"

WifiManager::WifiManager(const char *ssid,
                         const char *passwd,
                         EventFamily wifiEventFamily)
    : EventSource(wifiEventFamily),
      ssid(ssid), passwd(passwd),
      state(WifiState::Disconnected) {}

bool WifiManager::generateStateEvent()
{
  return this->generateEvent(this->state);
}

void WifiManager::switchState(WifiState state)
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
  this->switchState(WifiState::Connected);
}

void WifiManager::onDisconnect(arduino_event_id_t id, arduino_event_info_t info)
{
  this->switchState(WifiState::Disconnected);
}

void WifiManager::begin(EventsManager *eventsManager)
{
  EventSource<WifiState>::begin(eventsManager);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.onEvent([this](arduino_event_id_t id, arduino_event_info_t info)
               { this->onConnect(id, info); }, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent([this](arduino_event_id_t id, arduino_event_info_t info)
               { this->onDisconnect(id, info); }, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.begin(this->ssid, this->passwd);
  this->generateStateEvent();
}
