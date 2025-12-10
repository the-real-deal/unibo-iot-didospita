#include "state.hpp"
#include <assert.h>

StateManager::StateManager(StateType initialState)
    : state(initialState), previousState(initialState),
      stateCandidate(initialState), hasPrevious(false) {}

StateType StateManager::getState() { return this->state; }

bool StateManager::hasPreviousState() { return this->hasPrevious; }

StateType StateManager::getPreviousState() {
  assert(this->hasPreviousState());
  return this->previousState;
}

void StateManager::setState(StateType state) { this->stateCandidate = state; }

void StateManager::switchState() {
  this->previousState = this->state;
  this->state = this->stateCandidate;
  this->hasPrevious = true;
}
