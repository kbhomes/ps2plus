#include "test_commands.h"

const uint8_t COMMAND_ID = 0x4C;
const size_t PAYLOAD_SIZE = 6;

void test_device_descriptor_4c_command_first_byte() {
  uint8_t command_input[PAYLOAD_SIZE] = { 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, };
  uint8_t expected_output[PAYLOAD_SIZE] = { 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, };
  uint8_t *actual_output = helper_run_command(&state, COMMAND_ID, command_input, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, PAYLOAD_SIZE);
}

void test_device_descriptor_4c_command_second_byte() {
  uint8_t command_input[PAYLOAD_SIZE] = { 0x01, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, };
  uint8_t expected_output[PAYLOAD_SIZE] = { 0x00, 0x00, 0x00, 0x07, 0x00, 0x10, };
  uint8_t *actual_output = helper_run_command(&state, COMMAND_ID, command_input, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, PAYLOAD_SIZE);
}