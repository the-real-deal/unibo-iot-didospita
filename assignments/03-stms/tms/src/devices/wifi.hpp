#pragma once

#include <WiFi.h>

#include "core/wifi.hpp"
#include "kernel/events.hpp"

class WifiManager: public EventSource<WifiStatusChangeEvent>
{
private:
    const char *ssid;
    const char *passwd;
    WiFiClient wifiClient;

public:
    WifiManager(const char *ssid, const char *passwd);
};
