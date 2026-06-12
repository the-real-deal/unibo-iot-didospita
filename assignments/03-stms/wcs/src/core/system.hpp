#pragma once

enum class SystemStatus
{
    Manual,
    Automatic,
    Unconnected,
};
const char *const SYSTEM_STATUS_STRINGS[] = {
    "MANUAL",
    "AUTOMATIC",
    "UNCONNECTED",
};

struct SystemStatusChangeEvent
{
    SystemStatus status;
    SystemStatus prev;
};
