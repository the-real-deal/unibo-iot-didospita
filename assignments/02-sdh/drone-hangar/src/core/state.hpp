#pragma once

#include "serial.hpp"
#include <Arduino.h>

enum class StateType {
  INSIDE,
  OUTSIDE,
  ALARM,
};
const String STATE_TYPE_STRINGS[] = {
    "INSIDE",
    "OUTSIDE",
    "ALARM",
};

class StateManager {
private:
  StateType state;
  SerialManager *serialManager;

public:
  StateManager(StateType state, SerialManager *serialManager);
  void setState(StateType state);
  StateType getState();
};
