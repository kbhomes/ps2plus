#include "test_commands.h"

const uint8_t COMMAND_ID = 0x40;
const size_t PAYLOAD_SIZE = 6;

void test_initialize_pressure_sensor_command_constant_response() {
  uint8_t command_input[PAYLOAD_SIZE] = { 0x00, 0x02, 0x00, 0x00, 0x00, 0x00 };
  uint8_t expected_output[PAYLOAD_SIZE] = { 0x00, 0x00, 0x02, 0x00, 0x00, 0x5A };
  uint8_t *actual_output = helper_run_command(&state, COMMAND_ID, command_input, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, PAYLOAD_SIZE);
}