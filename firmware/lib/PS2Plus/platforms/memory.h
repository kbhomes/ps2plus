#ifndef PLATFORMS_MEMORY_H
#define PLATFORMS_MEMORY_H

#include <stddef.h>
#include <stdint.h>

size_t platform_memory_total_size();
uint8_t platform_memory_read(size_t address);
void platform_memory_write(size_t address, uint8_t value);

// All methods below have a common implementation based on `platform_memory_read/write`,
// implemented in `platform.c`
uint16_t platform_memory_read_short(size_t address);
uint32_t platform_memory_read_int(size_t address);
void platform_memory_read_array(size_t address, uint8_t *dst, size_t count);
void platform_memory_write_short(size_t address, uint16_t value);
void platform_memory_write_int(size_t address, uint32_t value);
void platform_memory_write_array(size_t address, uint8_t *src, size_t count);

#endif /* PLATFORMS_MEMORY_H */