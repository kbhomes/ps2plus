#ifndef CONTROLLER_INPUT_H
#define CONTROLLER_INPUT_H

#include <stdbool.h>
#include <stdint.h>

#include <shared/controller.h>
#include <utilities/debounced_bool.h>

#define NUM_PRESSURE_SENSITIVE_BUTTONS 12
extern ps2plus_controller_digital_button DIGITAL_BUTTON_TO_PRESSURE_INDEX_MAP[NUM_PRESSURE_SENSITIVE_BUTTONS];

/**
 * @brief Input state of the emulated PS2 controller
 */
typedef struct {
  debounced_bool digital_buttons[NUM_DIGITAL_BUTTONS];
  uint8_t joysticks[NUM_JOYSTICK_AXES];
  uint8_t button_data[NUM_DIGITAL_BUTTONS/8 + NUM_JOYSTICK_AXES + NUM_PRESSURE_SENSITIVE_BUTTONS];
} controller_input;

void controller_input_initialize(controller_input *input);
uint16_t controller_input_as_digital(controller_input *input);
void controller_input_recompute(controller_input *input);

#endif /* CONTROLLER_INPUT_H */