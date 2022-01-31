#ifdef PLATFORM_NATIVE

#include "native_platform.h"

bool platform_controller_read_digital_button(controller_input_digital_button button) {
  return false;
}

uint8_t platform_controller_read_joystick(controller_input_joystick_axis joystick_axis) {
  return 0x7F;
}

void platform_controller_set_analog_led(bool active) {
  
}

#endif