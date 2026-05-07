#include "debug.hpp"

extern int __heap_start, *__brkval;

int freeMemory()
{
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}