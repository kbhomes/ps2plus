#ifdef PLATFORM_ARDUINO

#include "arduino_platform.h"

// Mapping from `controller_input_digital_button` to an Arduino Mega pin number
const int DIGITAL_BUTTON_PINS[NUM_DIGITAL_BUTTONS] = {
  33, // Select
  46, // L3
  47, // R3
  31, // Start
  41, // Up
  39, // Right
  37, // Down
  35, // Left
  44, // L2
  45, // R2
  43, // L1
  42, // R1
  40, // Triangle
  38, // Circle
  36, // Cross
  34, // Square
};

void arduino_setup_wired_controller() {
  // Setup each digital button as an active-low button
  for (int i = 0; i < NUM_DIGITAL_BUTTONS; i++) {
    pinMode(DIGITAL_BUTTON_PINS[i], INPUT_PULLUP);
  }
}

bool platform_controller_read_digital_button(controller_input_digital_button button) {
#ifdef PLATFORM_ARDUINO_MEGA
  return digitalRead(DIGITAL_BUTTON_PINS[button]) == LOW;
#else
  return false;
#endif
}

uint8_t platform_controller_read_joystick(controller_input_joystick_axis joystick_axis) {
  return 0x7F;
}

void platform_controller_set_analog_led(bool active) {
  
}

#endif