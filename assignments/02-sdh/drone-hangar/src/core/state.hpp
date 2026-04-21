#pragma once

#include <Arduino.h>

enum class GlobalState
{
  Inside,
  Outside,
  Prealarm,
  Alarm,
  Takeoff,
  Landing,
};
const char *const GLOBAL_STATE_STRINGS[] = {
    "DRONE INSIDE",
    "DRONE OUT",
    "PREALARM",
    "ALARM",
    "TAKE OFF",
    "LANDING",
};
