#include "test_commands.h"

#include <string.h>

#define PS2PLUS_GC_COMMAND_ID 0x71

void test_command_ps2plus_get_configuration_joystick_axis_range_remapping() {
  uint8_t axis_mappings[NUM_JOYSTICK_AXIS_RANGES] = { 10, 20, 30, 11, 21, 31, 12, 22, 32, 13, 23, 33 };
  memcpy(&state.custom_config.joystick_axis_range_remapping_values, axis_mappings, NUM_JOYSTICK_AXIS_RANGES);

  for (controller_joystick_axis_range_remapping jsar = 0; jsar < NUM_JOYSTICK_AXIS_RANGES; jsar++) {
    // Request the specific joystick axis range remapping value
    uint8_t command_input[] = { jsar & 0xFF, (jsar >> 8) & 0xFF, 0x00, 0x00 };
    uint8_t expected_output[] = { 0x00, 0x00, 0x02, axis_mappings[jsar] };
    uint8_t *actual_output = helper_run_command(&state, PS2PLUS_GC_COMMAND_ID, command_input, sizeof(command_input));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, sizeof(command_input));
  }
}

void test_command_ps2plus_get_configuration_joystick_digital_mode_enabled() {
  bool values[] = { false, true };

  for (size_t i = 0; i < sizeof(values); i++) {
    state.custom_config.joystick_digital_mode_enabled = values[i];
    uint8_t command_input[] = { 0x0C, 0x00, 0x00, 0x00 };
    uint8_t expected_output[] = { 0x00, 0x00, 0x01, (values[i] ? 0xFF : 0x00) };
    uint8_t *actual_output = helper_run_command(&state, PS2PLUS_GC_COMMAND_ID, command_input, sizeof(command_input));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, sizeof(command_input));
  }
}

void test_command_ps2plus_get_configuration_global_button_remapping_enabled() {
  bool values[] = { false, true };

  for (size_t i = 0; i < sizeof(values); i++) {
    state.custom_config.global_button_remapping_enabled = values[i];
    uint8_t command_input[] = { 0x0D, 0x00, 0x00, 0x00 };
    uint8_t expected_output[] = { 0x00, 0x00, 0x01, (values[i] ? 0xFF : 0x00) };
    uint8_t *actual_output = helper_run_command(&state, PS2PLUS_GC_COMMAND_ID, command_input, sizeof(command_input));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, sizeof(command_input));
  }
}