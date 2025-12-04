#pragma once

#include "scheduler.hpp"

template <typename T> class Sensor : public ExternalInput {
protected:
  T value;

public:
  T getValue() { return this->value; };
};
