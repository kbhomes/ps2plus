#ifdef PLATFORM_ARDUINO

#include "arduino_platform.h"

// Interrupt callback 
platform_interrupt_callback interrupt_callback = NULL;

void platform_init(platform_interrupt_callback callback) {
  init();

  interrupt_callback = callback;
  arduino_setup_spi_playstation();
  arduino_setup_uart_serial();

#ifdef UNIT_TEST
  // Unit tests on an Arduino board require a delay during startup
  delay(2000);
#endif
}

#endif