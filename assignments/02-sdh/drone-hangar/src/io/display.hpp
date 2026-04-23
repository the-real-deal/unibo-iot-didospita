#pragma once

#include <Arduino.h>

class Display
{
public:
  virtual void clear();
  virtual void print(const char *message);
  virtual ~Display() = default;
};
