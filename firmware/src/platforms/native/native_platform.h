#ifndef NATIVE_PLATFORM_H
#define NATIVE_PLATFORM_H

#include <platforms/platform.h>

/**
 * @brief Interrupt callback provided by the main program, to be called by the platform's SPI
 *        interrupt when data is available to read from the console
 */
extern platform_interrupt_callback main_loop_callback;

#endif