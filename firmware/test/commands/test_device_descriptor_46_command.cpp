#include "test_commands.h"

const uint8_t COMMAND_ID = 0x46;
const size_t PAYLOAD_SIZE = 6;

void test_device_descriptor_46_command_first_byte() {
  uint8_t command_input[PAYLOAD_SIZE] = { 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, };
  uint8_t expected_output[PAYLOAD_SIZE] = { 0x00, 0x00, 0x01, 0x02, 0x00, 0x0A, };
  uint8_t *actual_output = helper_run_command(&state, COMMAND_ID, command_input, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, PAYLOAD_SIZE);
}

void test_device_descriptor_46_command_second_byte() {
  uint8_t command_input[PAYLOAD_SIZE] = { 0x01, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, };
  uint8_t expected_output[PAYLOAD_SIZE] = { 0x00, 0x00, 0x01, 0x01, 0x01, 0x14, };
  uint8_t *actual_output = helper_run_command(&state, COMMAND_ID, command_input, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, PAYLOAD_SIZE);
}