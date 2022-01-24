#ifdef PLATFORM_NATIVE

#include <math.h>
#include <time.h>
#include <unistd.h>

#include <platforms/platform.h>

void platform_init(platform_interrupt_callback main_loop_callback) {
  
}

unsigned long platform_timing_millis() {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return now.tv_sec * 1000 + lround(now.tv_nsec / 1e6);
}

void platform_timing_sleep(unsigned int millis) {
  usleep(1000 * millis);
}

bool platform_spi_selected() {
  return false;
}

bool platform_spi_data_available() {
  return false;
}

uint8_t platform_spi_read() {
  return 0;
}

void platform_spi_write(uint8_t value) {

}

uint8_t platform_memory_read(size_t address) {
  return 0;
}

void platform_memory_write(size_t address, uint8_t byte) {

}

#endif