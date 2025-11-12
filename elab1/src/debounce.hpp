#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <config.h>
#include <stdint.h>

typedef void (*debounce_func_t)();

bool check_debounce(uint64_t interval_ms);
bool check_debounce_default();

#endif // DEBOUNCE_H