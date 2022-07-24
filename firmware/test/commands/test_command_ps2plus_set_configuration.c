#include "suite.h"

void test_command_ps2plus_set_configuration_valid() {
  // Updates a configuration value to a non-default value
  uint8_t expected_value = 0xAA;
  uint16_t id = CONFIGURATION_ID(joystick_deadzone_left);

  // Requests the specified configuration
  uint8_t command_input[] = { id & 0xFF, (id >> 8) & 0xFF, PDT_Uint8, expected_value };

  // 0x00 values indicate a valid request
  uint8_t expected_output[] = { 0x00, 0x00, 0x00, 0x00 };
  uint8_t *actual_output = helper_run_command(command_ps2plus_set_configuration.id, command_input, sizeof(expected_output));
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, sizeof(expected_output));
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(state.custom_config.values.joystick_deadzone_left.uint8, expected_value, "Configuration was not updated with the new value");
}

void test_command_ps2plus_set_configuration_unknown() {
  // Specifies a configuration ID that doesn't exist
  uint16_t id = NUM_CUSTOM_CONFIGURATIONS + 1;

  // Tries to update the invalid configuration with a uint8_t value
  uint8_t command_input[] = { id & 0xFF, (id >> 8) & 0xFF, PDT_Uint8, 0xAA };

  // 0xFF values indicate an invalid request
  uint8_t expected_output[] = { 0x00, 0x00, 0xFF, 0xFF };
  uint8_t *actual_output = helper_run_command(command_ps2plus_set_configuration.id, command_input, sizeof(expected_output));
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, sizeof(expected_output));
}