#include "controller_input.h"

#define DEBOUNCE_DIGITAL_BUTTON_MS 2

controller_input_digital_button DIGITAL_BUTTON_TO_PRESSURE_INDEX_MAP[NUM_PRESSURE_SENSITIVE_BUTTONS] = {
  DDRight, DDLeft, DDUp, DDDown,
  DBTriangle, DBCircle, DBCross, DBSquare,
  DBL1, DBR1, DBL2, DBR2,
};

void controller_input_initialize(controller_input *input) {
  // Write each digital button value as an active-low bit
  for (int i = 0; i < NUM_DIGITAL_BUTTONS; i++) {
    debounced_init(&input->digital_buttons[i], 1, DEBOUNCE_DIGITAL_BUTTON_MS);
  }

  // Set mid-axis values for each joystick
  for (int j = 0; j < NUM_JOYSTICK_AXES; j++) {
    input->joysticks[j] = 0x7F;
  }
}

uint16_t controller_input_as_digital(controller_input *input) {
  uint16_t output = 0;

  // Write each digital button value as an active-low bit
  for (int i = 0; i < NUM_DIGITAL_BUTTONS; i++) {
    int value = debounced_read(&input->digital_buttons[i]);
    int shift = NUM_DIGITAL_BUTTONS - i - 1;
    output |= (value << shift);
  }

  return output;
}