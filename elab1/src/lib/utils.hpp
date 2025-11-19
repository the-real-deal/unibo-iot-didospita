#pragma once

#include <Arduino.h>
#include <stdlib.h>

template <typename T>
size_t indexOf(const T *arr, const size_t length, const T value) {
  for (size_t i = 0; i < length; i++) {
    if (arr[i] == value) {
      return i;
    }
  }
  return (size_t)-1; // not found
}

void deepSleep();
void disableSleep();
