#pragma once

#include <Arduino.h>

template <typename T>
const char* const enumToString(T value, const char* const enumStrings[]) {
  return enumStrings[static_cast<size_t>(value)];
}

template <typename T, size_t N>
T enumFromString(const String value, const char* const (&enumStrings)[N]) {
  for (size_t i = 0; i < N; ++i) {
    if (value == String(enumStrings[i])) return static_cast<T>(i);
  }
  abort();
}

template <typename T>
void safeDelete(T** ptr) {
  if (*ptr == nullptr) {
    delete *ptr;
    *ptr = nullptr;
  }
}
