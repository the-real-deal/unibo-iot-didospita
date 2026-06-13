#pragma once

enum class SystemState
{
    Manual,
    Automatic,
    Unconnected,
};
const char *const SYSTEM_STATE_STRINGS[] = {
    "MANUAL",
    "AUTOMATIC",
    "UNCONNECTED",
};
