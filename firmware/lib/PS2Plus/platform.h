#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/**
 * @brief Initialize the executing platform as required for runtime execution
 *        as well as unit tests
 */
void platform_init(void);

/**
 * @brief Returns a millisecond-resolution system time
 */
unsigned long platform_millis(void);

/**
 * @brief Attempts to sleep for the provided number of milliseconds 
 */
void platform_sleep(unsigned int millis);

uint8_t platform_spi_read();
void platform_spi_write(uint8_t);

#endif /* PLATFORM_H */