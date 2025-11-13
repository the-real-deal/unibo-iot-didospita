#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <config.h>
#include <stdint.h>

typedef void (*debounce_func_t)();

bool checkDebounce(uint64_t interval_ms);
bool checkDebounce();

#endif // DEBOUNCE_H