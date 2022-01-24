#ifdef PLATFORM_PIC16F

#include "pic16f_platform.h"

// Mapping from `controller_input_digital_button` to a digital pin
const pic_digital_io_pin DIGITAL_BUTTON_PINS[NUM_DIGITAL_BUTTONS] = {
  PIC_DIGITAL_IO_PIN(B, 3), // Select
  PIC_DIGITAL_IO_PIN(B, 2), // L3
  PIC_DIGITAL_IO_PIN(A, 3), // R3
  PIC_DIGITAL_IO_PIN(A, 2), // Start
  PIC_DIGITAL_IO_PIN(D, 4), // Up
  PIC_DIGITAL_IO_PIN(D, 5), // Right
  PIC_DIGITAL_IO_PIN(D, 7), // Down
  PIC_DIGITAL_IO_PIN(D, 6), // Left
  PIC_DIGITAL_IO_PIN(C, 7), // L2
  PIC_DIGITAL_IO_PIN(C, 1), // R2
  PIC_DIGITAL_IO_PIN(C, 6), // L1
  PIC_DIGITAL_IO_PIN(C, 2), // R1
  PIC_DIGITAL_IO_PIN(C, 0), // Triangle
  PIC_DIGITAL_IO_PIN(A, 7), // Circle
  PIC_DIGITAL_IO_PIN(E, 2), // Cross
  PIC_DIGITAL_IO_PIN(A, 6), // Square
};

const pic_digital_io_pin PIN_LED_ANALOG_MODE = PIC_DIGITAL_IO_PIN(B, 4);
const pic_digital_io_pin PIN_MOTOR_SMALL = PIC_DIGITAL_IO_PIN(A, 0);
const pic_digital_io_pin PIN_MOTOR_LARGE = PIC_DIGITAL_IO_PIN(B, 5);

const pic_analog_io_pin ANALOG_JOYSTICK_PINS[NUM_JOYSTICK_AXES] = {
  { .bit = 0, .enable = &ANSELE, .channel = 0b100000 }, // RX
  { .bit = 1, .enable = &ANSELE, .channel = 0b100001 }, // RY
  { .bit = 1, .enable = &ANSELB, .channel = 0b001001 }, // LX
  { .bit = 0, .enable = &ANSELB, .channel = 0b001000 }, // LY
};

void pic16f_setup_wired_controller() {
  // Setup each digital button as an active-low input button
  for (int i = 0; i < NUM_DIGITAL_BUTTONS; i++) {
    pic_digital_io_pin_mode(&DIGITAL_BUTTON_PINS[i], PICPinMode_InputPullup);
  }
  
  pic_digital_io_pin_mode(&PIN_LED_ANALOG_MODE, PICPinMode_Output);
  pic_digital_io_pin_mode(&PIN_MOTOR_SMALL, PICPinMode_Output);
  pic_digital_io_pin_mode(&PIN_MOTOR_LARGE, PICPinMode_Output);

  
  // Setup each joystick axis as an analog input
  for (int i = 0; i < NUM_JOYSTICK_AXES; i++) {
    *(ANALOG_JOYSTICK_PINS[i].enable) |= (PIC_ANALOG_ENABLE << ANALOG_JOYSTICK_PINS[i].bit);
  }
}

bool platform_controller_read_digital_button(controller_input_digital_button button) {
  return pic_digital_io_pin_read(&DIGITAL_BUTTON_PINS[button]) == PIC_DIGITAL_LOW;
}

uint8_t platform_controller_read_joystick(controller_input_joystick_axis joystick_axis) {
  ADPCH = ANALOG_JOYSTICK_PINS[joystick_axis].channel;
  ADPRE = 0x00;
  ADACQ = 0x3F;
  
  // Kick off ADC conversion and wait for it to complete
  ADGO = 1;
  while (ADGO);

  // Returns an 8-bit converted result
  return ((ADRESH << 6) | (ADRESL >> 2)); 
}

void platform_controller_set_analog_led(bool active) {
  pic_digital_io_pin_write(&PIN_LED_ANALOG_MODE, active);
}

#endif