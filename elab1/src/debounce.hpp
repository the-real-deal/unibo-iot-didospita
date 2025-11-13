#ifndef DEBOUNCE_HPP
#define DEBOUNCE_HPP

#include "config.h"
#include <stdint.h>

typedef void (*debounce_func_t)();

bool checkDebounce(uint64_t interval_ms = DEFAULT_DEBOUNCE_MS);

#endif // DEBOUNCE_HPP
