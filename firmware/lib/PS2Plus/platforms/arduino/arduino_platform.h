#ifndef ARDUINO_PLATFORM_H
#define ARDUINO_PLATFORM_H

#include <platforms/platform.h>
#include <Arduino.h>

#define PIN_ACK 4

/**
 * @brief Interrupt callback provided by the main program, to be called by the platform's SPI
 *        interrupt when data is available to read from the console
 */
extern platform_interrupt_callback interrupt_callback;

void arduino_setup_spi_playstation();
void arduino_setup_uart_serial();
void arduino_setup_wired_controller();

#endif