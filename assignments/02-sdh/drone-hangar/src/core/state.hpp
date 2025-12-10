#pragma once

#include <Arduino.h>

enum class StateType {
  INSIDE,
  OUTSIDE,
  ALARM,
  TAKEOFF,
  LANDING,
};
const String STATE_TYPE_STRINGS[] = {
    "DRONE INSIDE", "DRONE OUTSIDE", "ALARM", "TAKEOFF", "LANDING",
};

class StateManager {
private:
  StateType state;
  StateType previousState;
  StateType stateCandidate;
  bool hasPrevious;

public:
  StateManager(StateType initialState);
  StateType getState();
  bool hasPreviousState();
  StateType getPreviousState();
  void setState(StateType state);
  void switchState();
};
