#ifdef PLATFORM_NATIVE

#include "native_platform.h"

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

#endif /* PLATFORM_NATIVE */