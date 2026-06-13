#pragma once

#include <WiFi.h>

#include "core/network.hpp"
#include "kernel/events.hpp"

#ifndef WIFI_MAX_MANAGERS
#define WIFI_MAX_MANAGERS 1
#endif

class WifiManager : public EventSource<NetworkState>
{
private:
    const char *ssid;
    const char *passwd;
    NetworkState state;
    WiFiClient client;
    bool eventsAttached;
    wifi_event_id_t onConnectEvent;
    wifi_event_id_t onDisconnectEvent;

    bool generateStateEvent();
    void switchState(NetworkState state);
    void onConnect(arduino_event_id_t id, arduino_event_info_t info);
    void onDisconnect(arduino_event_id_t id, arduino_event_info_t info);

public:
    WifiManager(const char *ssid, const char *passwd, EventFamily networkStateEventFamily);
    ~WifiManager();
    void begin(EventsManager *eventsManager) override;
    WiFiClient &getClient();
};
