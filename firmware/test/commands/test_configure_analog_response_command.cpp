#include "test_commands.h"

const uint8_t COMMAND_ID = 0x41;
const size_t PAYLOAD_SIZE = 6;
const uint8_t COMMAND_INPUT[PAYLOAD_SIZE] = { 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A };
const uint8_t EXPECTED_DIGITAL_OUTPUT[PAYLOAD_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const uint8_t EXPECTED_ANALOG_OUTPUT[PAYLOAD_SIZE] = { 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x5A };

void test_button_inclusions_command_digital_mode() {
  state.analog_mode = CMDigital;

  uint8_t *actual_output = helper_run_command(&state, COMMAND_ID, COMMAND_INPUT, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(EXPECTED_DIGITAL_OUTPUT, actual_output, PAYLOAD_SIZE);
}

void test_button_inclusions_command_analog_mode() {
  state.analog_mode = CMAnalog;

  uint8_t *actual_output = helper_run_command(&state, COMMAND_ID, COMMAND_INPUT, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(EXPECTED_ANALOG_OUTPUT, actual_output, PAYLOAD_SIZE);
}

void test_button_inclusions_command_analog_full_mode() {
  state.analog_mode = CMAnalogFull;

  uint8_t *actual_output = helper_run_command(&state, COMMAND_ID, COMMAND_INPUT, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(EXPECTED_ANALOG_OUTPUT, actual_output, PAYLOAD_SIZE);
}