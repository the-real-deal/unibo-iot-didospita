#pragma once

#include <Arduino.h>

enum class GlobalState {
  Inside,
  Outside,
  Prealarm,
  Alarm,
  Takeoff,
  Landing,
};
const String GLOBAL_STATE_STRINGS[] = {
    "DRONE INSIDE", "DRONE OUTSIDE", "PREALARM", "ALARM", "TAKEOFF", "LANDING",
};
