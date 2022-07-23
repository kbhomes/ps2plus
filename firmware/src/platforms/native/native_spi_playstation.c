#ifdef PLATFORM_NATIVE

#include "native_platform.h"

void platform_spi_playstation_ack() {

}

bool platform_spi_playstation_selected() {
  return false;
}

bool platform_spi_playstation_data_available() {
  return false;
}

uint8_t platform_spi_playstation_read() {
  return 0;
}

void platform_spi_playstation_write(uint8_t value) {

}

#endif /* PLATFORM_NATIVE */