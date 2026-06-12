#pragma once

enum class SystemStatus
{
    Manual,
    Automatic,
};
const char *const SYSTEM_STATUS_STRINGS[] = {
    "MANUAL",
    "AUTOMATIC",
};

struct SystemStatusChangeEvent
{
    SystemStatus status;
    SystemStatus prev;
};
