#pragma once

#include <Arduino.h>

enum class GlobalState {
  Inside,
  Outside,
  Alarm,
  Takeoff,
  Landing,
};
const String STATE_TYPE_STRINGS[] = {
    "DRONE INSIDE", "DRONE OUTSIDE", "ALARM", "TAKEOFF", "LANDING",
};
