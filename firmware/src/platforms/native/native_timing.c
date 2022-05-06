#ifdef PLATFORM_NATIVE

#include "native_platform.h"

#include <math.h>
#include <time.h>
#include <unistd.h>

unsigned long platform_timing_millis() {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return now.tv_sec * 1000 + lround(now.tv_nsec / 1e6);
}

void platform_timing_sleep(unsigned int millis) {
  usleep(1000 * millis);
}

#endif /* PLATFORM_NATIVE */