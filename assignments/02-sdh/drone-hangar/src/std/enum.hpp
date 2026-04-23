#pragma once

#include <Arduino.h>

template <typename T>
const char *enumToString(T value, const char *const enumStrings[])
{
  auto strRepr = enumStrings[static_cast<size_t>(value)];
  return strRepr;
}

template <typename T, size_t N>
T enumFromString(const char *value, const char *const (&enumStrings)[N])
{
  for (size_t i = 0; i < N; ++i)
  {
    if (strcmp(value, enumStrings[i]) == 0)
    {
      auto enumVal = static_cast<T>(i);
      return enumVal;
    }
  }
  abort();
}
