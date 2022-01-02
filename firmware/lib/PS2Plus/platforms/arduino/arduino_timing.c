#ifdef PLATFORM_ARDUINO

#include "arduino_platform.h"

unsigned long platform_timing_millis(void) {
  return millis();
}

void platform_timing_sleep(unsigned int millis) {
  delay(millis);
}

#endif /* PLATFORM_ARDUINO */