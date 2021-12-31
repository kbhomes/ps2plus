#ifndef PLATFORMS_MEMORY_H
#define PLATFORMS_MEMORY_H

#include <stddef.h>
#include <stdint.h>

uint8_t platform_memory_read(size_t address);
void platform_memory_write(size_t address, uint8_t value);

#endif /* PLATFORMS_MEMORY_H */