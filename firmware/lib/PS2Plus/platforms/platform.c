#include "platform.h"

// Implementations common to all platforms

uint16_t platform_memory_read_short(size_t address) {
  return platform_memory_read(address) | (platform_memory_read(address + 1) << 8);
}

void platform_memory_write_short(size_t address, uint16_t value) {
  platform_memory_write(address, value & 0xFF);
  platform_memory_write(address + 1, value >> 8);
}

uint32_t platform_memory_read_int(size_t address) {
  return (
    ((uint32_t)platform_memory_read(address)) | 
    ((uint32_t)platform_memory_read(address + 1) << 8) |
    ((uint32_t)platform_memory_read(address + 2) << 16) | 
    ((uint32_t)platform_memory_read(address + 3) << 24)
  );
}

void platform_memory_write_int(size_t address, uint32_t value) {
  platform_memory_write(address, value & 0xFF);
  platform_memory_write(address + 1, (value >> 8) & 0xFF);
  platform_memory_write(address + 2, (value >> 16) & 0xFF);
  platform_memory_write(address + 3, (value >> 24) & 0xFF);
}

void platform_memory_read_array(size_t address, uint8_t *dst, size_t count) {
  while (count--) {
    *dst++ = platform_memory_read(address++);
  }
}

void platform_memory_write_array(size_t address, uint8_t *src, size_t count) {
  while (count--) {
    platform_memory_write(address++, *src++);
  }
}