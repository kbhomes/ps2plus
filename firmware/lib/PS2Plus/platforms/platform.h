#ifndef PLATFORMS_PLATFORM_H
#define PLATFORMS_PLATFORM_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "controller.h"
#include "memory.h"
#include "timing.h"
#include "spi_playstation.h"

/**
 * What does any given platform need?
 *   
 * - Utility functions:
 *   - Timing and sleep
 *   - Non-volatile memory read/write
 * 
 * - System setup:
 *   - Configuring the MCU, pins, etc.
 *   - Enabling SPI, interrupts, etc.
 * 
 * - SPI support:
 *   - Reading the latest byte from the console
 *   - Writing a new byte to the console
 * 
 * - Command processing hooks:
 *   - Ability for the platform to hook into the 
 *     command processing loop
 */

typedef void (*platform_interrupt_callback)(void);

/**
 * @brief Initialize the executing platform as required for runtime execution
 *        as well as unit tests
 */
void platform_init(platform_interrupt_callback);

#endif /* PLATFORMS_PLATFORM_H */