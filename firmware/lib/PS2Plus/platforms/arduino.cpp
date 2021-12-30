#ifdef PLATFORM_ARDUINO

#include <Arduino.h>

#include "../platform.h"

void platform_init(void) {
  init();

#ifdef UNIT_TEST
  delay(2000);
#endif
}

unsigned long platform_millis(void) {
  return millis();
}

void platform_sleep(unsigned int millis) {
  delay(millis);
}

uint8_t platform_spi_read() {
  return Serial.read() & 0xFF;
}

void platform_spi_write(uint8_t value) {
  Serial.write(value);
}

#endif