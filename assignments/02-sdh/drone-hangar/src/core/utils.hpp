#pragma once

#include <Arduino.h>
#include <assert.h>

template <typename T> String enumToString(T value, const String enumStrings[]) {
  return enumStrings[static_cast<size_t>(value)];
}

template <typename T, size_t N>
T enumFromString(const String &value, const String (&enumStrings)[N]) {
  for (size_t i = 0; i < N; ++i) {
    if (value == enumStrings[i])
      return static_cast<T>(i);
  }
  assert(false);
  return static_cast<T>(0); // avoid "no return" when asserts are disabled
}
