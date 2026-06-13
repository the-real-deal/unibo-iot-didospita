#include "wifi.hpp"

WifiManager::WifiManager(const char *ssid, const char *passwd)
    : ssid(ssid), passwd(passwd), wifiClient() {}
