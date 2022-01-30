#include "test_commands.h"

#define CAR_COMMAND_ID 0x4F
#define CAR_PAYLOAD_SIZE 6

const uint8_t CAR_COMMAND_INPUT[CAR_PAYLOAD_SIZE] = { 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00 };
const uint8_t CAR_EXPECTED_OUTPUT[CAR_PAYLOAD_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A };

void test_command_controller_configure_analog_response_digital_mode() {
  state.analog_mode = CMDigital;

  uint8_t *actual_output = helper_run_command(&state, CAR_COMMAND_ID, CAR_COMMAND_INPUT, CAR_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(CAR_EXPECTED_OUTPUT, actual_output, CAR_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_MESSAGE(CMDigital, state.analog_mode, "Controller should remain in digital mode");
}

void test_command_controller_configure_analog_response_analog_mode() {
  state.analog_mode = CMAnalog;

  uint8_t *actual_output = helper_run_command(&state, CAR_COMMAND_ID, CAR_COMMAND_INPUT, CAR_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(CAR_EXPECTED_OUTPUT, actual_output, CAR_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_MESSAGE(CMAnalogFull, state.analog_mode, "Controller should be in full analog mode");
}

void test_command_controller_configure_analog_response_analog_full_mode() {
  state.analog_mode = CMAnalogFull;

  uint8_t *actual_output = helper_run_command(&state, CAR_COMMAND_ID, CAR_COMMAND_INPUT, CAR_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(CAR_EXPECTED_OUTPUT, actual_output, CAR_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_MESSAGE(CMAnalogFull, state.analog_mode, "Controller should be in full analog mode");
}