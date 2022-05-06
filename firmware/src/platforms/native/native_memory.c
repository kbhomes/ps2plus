#ifdef PLATFORM_NATIVE

#include "native_platform.h"

#define NATIVE_SIMULATED_EEPROM_SIZE 0x400

uint8_t storage[NATIVE_SIMULATED_EEPROM_SIZE];

size_t platform_memory_total_size() {
  return NATIVE_SIMULATED_EEPROM_SIZE;
}

uint8_t platform_memory_read(size_t address) {
  return storage[address];
}

void platform_memory_write(size_t address, uint8_t byte) {
  storage[address] = byte;
}

#endif /* PLATFORM_NATIVE */