#pragma once

enum class WifiStatus
{
    Connected,
    Unconnected,
};
const char *const WIFI_STATUS_STRINGS[] = {
    "CONNECTED",
    "UNCONNECTED",
};

struct WifiStatusChangeEvent
{
    WifiStatus status;
    WifiStatus prev;
};
