#include "state.hpp"

StateManager::StateManager(StateType state, SerialManager *serialManager)
    : state(state), serialManager(serialManager) {}

void StateManager::setState(StateType state) { this->state = state; }

StateType StateManager::getState() { return this->state; }