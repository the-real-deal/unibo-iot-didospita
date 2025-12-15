#pragma once

#include <Arduino.h>

enum class GlobalState {
  Inside,
  Outside,
  Alarm,
  Takeoff,
  Landing,
  RequestLanding,
};
const String GLOBAL_STATE_STRINGS[] = {
    "DRONE INSIDE", "DRONE OUTSIDE", "ALARM",
    "TAKEOFF",      "LANDING",       "REQUEST LANDING",
};
