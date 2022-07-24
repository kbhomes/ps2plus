#include "suite.h"

void test_command_ps2plus_get_configuration_valid() {
  // Updates a configuration value to a non-default value
  uint8_t expected_value = 0xAA;
  uint16_t id = CONFIGURATION_ID(joystick_deadzone_left);
  state.custom_config.values.joystick_deadzone_left.uint8 = expected_value;

  // Requests the specified configuration
  uint8_t command_input[] = { id & 0xFF, (id >> 8) & 0xFF, 0x00, 0x00 };
  uint8_t expected_output[] = { 0x00, 0x00, PDT_Uint8, expected_value };
  uint8_t *actual_output = helper_run_command(command_ps2plus_get_configuration.id, command_input, sizeof(command_input));
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, sizeof(command_input));
}

/**
 * Asserts that requesting the value for an unknown configuration returns an `Unknown` response
 */
void test_command_ps2plus_get_configuration_unknown() {
  // Specifies a configuration ID that doesn't exist
  uint16_t id = NUM_CUSTOM_CONFIGURATIONS + 1;
  uint8_t command_input[] = { id & 0xFF, (id >> 8) & 0xFF, 0x00 };
  uint8_t expected_output[] = { 0x00, 0x00, PDT_Unknown };
  uint8_t *actual_output = helper_run_command(command_ps2plus_get_configuration.id, command_input, sizeof(command_input));
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, sizeof(command_input));
}