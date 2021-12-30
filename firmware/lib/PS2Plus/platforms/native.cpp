#ifdef PLATFORM_NATIVE

#include <math.h>
#include <time.h>
#include <unistd.h>

#include "../platform.h"

void platform_init() {
  
}

unsigned long platform_millis() {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return now.tv_sec * 1000 + lround(now.tv_nsec / 1e6);
}

void platform_sleep(unsigned int millis) {
  usleep(1000 * millis);
}

uint8_t platform_spi_read() {
  return 0;
}

void platform_spi_write(uint8_t value) {
  
}

#endif