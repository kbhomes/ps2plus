#ifdef PLATFORM_NATIVE

#include "native_platform.h"

// Interrupt callback 
platform_interrupt_callback main_loop_callback = NULL;

void platform_init(platform_interrupt_callback callback) {
  main_loop_callback = callback;
}

#endif