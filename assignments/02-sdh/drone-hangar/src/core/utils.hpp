#pragma once

#include <Arduino.h>
#include <assert.h>

template <typename T> String enumToString(T value, const String enumStrings[]) {
  return enumStrings[static_cast<size_t>(value)];
}

template <typename T>
T enumFromString(String value, const String enumStrings[]) {
  size_t count = sizeof(enumStrings) / sizeof(String);
  for (size_t i = 0; i < count; i++) {
    if (type == enumStrings[i]) {
      return static_cast<T>(i);
    }
  }
  assert(false);
}
