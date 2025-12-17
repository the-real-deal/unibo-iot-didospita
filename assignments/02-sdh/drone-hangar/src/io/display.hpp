#pragma once

#include <Arduino.h>

class Display {
 public:
  virtual void clear();
  virtual void print(String* message);
  virtual ~Display() = default;
};
