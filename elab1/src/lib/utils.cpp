#include "utils.hpp"
#include <avr/sleep.h>
#include <stdint.h>

void deepSleep() {
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();
}

void disableSleep() { sleep_disable(); }
