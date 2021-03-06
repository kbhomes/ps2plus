#include "suite.h"

#define DD47_COMMAND_ID 0x47
#define DD47_PAYLOAD_SIZE 6

void test_command_controller_device_descriptor_47_only_byte() {
  uint8_t command_input[DD47_PAYLOAD_SIZE] = { 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, };
  uint8_t expected_output[DD47_PAYLOAD_SIZE] = { 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, };
  uint8_t *actual_output = helper_run_command(DD47_COMMAND_ID, command_input, DD47_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, DD47_PAYLOAD_SIZE);
}