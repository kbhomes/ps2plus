#include "input.h"

#define DEBOUNCE_DIGITAL_BUTTON_MS 2

controller_input_digital_button DIGITAL_BUTTON_TO_PRESSURE_INDEX_MAP[NUM_PRESSURE_SENSITIVE_BUTTONS] = {
  DDRight, DDLeft, DDUp, DDDown,
  DBTriangle, DBCircle, DBCross, DBSquare,
  DBL1, DBR1, DBL2, DBR2,
};

void controller_input_initialize(controller_input *input) {
  // Write each digital button value as an active-low bit
  for (int i = 0; i < NUM_DIGITAL_BUTTONS; i++) {
    debounced_init(&input->digital_buttons[i], true, DEBOUNCE_DIGITAL_BUTTON_MS);
  }

  // Set mid-axis values for each joystick
  for (int j = 0; j < NUM_JOYSTICK_AXES; j++) {
    input->joysticks[j] = 0x7F;
  }

  controller_input_recompute(input);
}

uint16_t controller_input_as_digital(controller_input *input) {
  uint16_t output = 0;

  // Write each digital button value as an active-low bit
  for (int i = 0; i < NUM_DIGITAL_BUTTONS; i++) {
    int value = debounced_read(&input->digital_buttons[i]);
    output |= (value << i);
  }

  return output;
}

/**
 * @brief Converts the current controller input into the array of bytes that will
 *        be sent to the console as part of the polling command
 */
void controller_input_recompute(controller_input *input) {
  int index = 0;

  // Store digital button information
  uint16_t digital = controller_input_as_digital(input);
  input->button_data[index++] = digital & 0xFF;
  input->button_data[index++] = (digital >> 8) & 0xFF;

  // Store analog joystick information
  for (int j = 0; j < NUM_JOYSTICK_AXES; j++) {
    input->button_data[index++] = input->joysticks[j];
  }

  // Store button pressure information (which will be converted to pure 00h or FFh)
  for (unsigned int b = 0; b < (sizeof(DIGITAL_BUTTON_TO_PRESSURE_INDEX_MAP) / sizeof(controller_input_digital_button)); b++) {
    controller_input_digital_button button = DIGITAL_BUTTON_TO_PRESSURE_INDEX_MAP[b];
    bool digital_value = debounced_read(&input->digital_buttons[button]);
    input->button_data[index++] = (digital_value == false) ? 0xFF : 0x00;
  }
}