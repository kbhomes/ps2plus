#include "test_commands.h"

#define BIC_COMMAND_ID 0x4F
#define BIC_PAYLOAD_SIZE 6

const uint8_t BIC_COMMAND_INPUT[BIC_PAYLOAD_SIZE] = { 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00 };
const uint8_t BIC_EXPECTED_OUTPUT[BIC_PAYLOAD_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A };

void test_configure_analog_response_command_digital_mode() {
  state.analog_mode = CMDigital;

  uint8_t *actual_output = helper_run_command(&state, BIC_COMMAND_ID, BIC_COMMAND_INPUT, BIC_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(BIC_EXPECTED_OUTPUT, actual_output, BIC_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_MESSAGE(CMDigital, state.analog_mode, "Controller should remain in digital mode");
}

void test_configure_analog_response_command_analog_mode() {
  state.analog_mode = CMAnalog;

  uint8_t *actual_output = helper_run_command(&state, BIC_COMMAND_ID, BIC_COMMAND_INPUT, BIC_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(BIC_EXPECTED_OUTPUT, actual_output, BIC_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_MESSAGE(CMAnalogFull, state.analog_mode, "Controller should be in full analog mode");
}

void test_configure_analog_response_command_analog_full_mode() {
  state.analog_mode = CMAnalogFull;

  uint8_t *actual_output = helper_run_command(&state, BIC_COMMAND_ID, BIC_COMMAND_INPUT, BIC_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(BIC_EXPECTED_OUTPUT, actual_output, BIC_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_MESSAGE(CMAnalogFull, state.analog_mode, "Controller should be in full analog mode");
}