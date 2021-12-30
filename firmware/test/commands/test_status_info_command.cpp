#include "test_commands.h"

const uint8_t COMMAND_ID = 0x45;
const size_t PAYLOAD_SIZE = 6;
const uint8_t COMMAND_INPUT[PAYLOAD_SIZE] = { 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, };

void test_status_info_command_digital_mode() {
  state.analog_mode = CMDigital;

  uint8_t expected_output[PAYLOAD_SIZE] = { 0x03, 0x02, 0x00, 0x02, 0x01, 0x00, };
  uint8_t *actual_output = helper_run_command(&state, COMMAND_ID, COMMAND_INPUT, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, PAYLOAD_SIZE);
}

void test_status_info_command_analog_mode() {
  state.analog_mode = CMAnalog;

  uint8_t expected_output[PAYLOAD_SIZE] = { 0x03, 0x02, 0x01, 0x02, 0x01, 0x00, };
  uint8_t *actual_output = helper_run_command(&state, COMMAND_ID, COMMAND_INPUT, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, PAYLOAD_SIZE);
}

void test_status_info_command_analog_full_mode() {
  state.analog_mode = CMAnalogFull;

  uint8_t expected_output[PAYLOAD_SIZE] = { 0x03, 0x02, 0x01, 0x02, 0x01, 0x00, };
  uint8_t *actual_output = helper_run_command(&state, COMMAND_ID, COMMAND_INPUT, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, PAYLOAD_SIZE);
}