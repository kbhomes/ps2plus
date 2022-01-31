#include "test_commands.h"

#include <string.h>

#define PS2PLUS_GV_COMMAND_ID 0x70

void test_command_ps2plus_get_version_firmware() {
  // Request firmware version (00h) and expect version 1 (0001h)
  uint8_t command_input[] = { 0x00, 0x00, 0x00, 0x00 };
  uint8_t expected_output[] = { 0x00, 0x03, 0x01, 0x00 };
  uint8_t *actual_output = helper_run_command(&state, PS2PLUS_GV_COMMAND_ID, command_input, sizeof(command_input));
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, sizeof(command_input));
}

void test_command_ps2plus_get_version_microcontroller() {
  const char mcu[] = "PIC18F46K42";
  const size_t mcu_size = 11;
  const size_t payload_size = 14;

  // Request microcontroller version (01h) and expect version "PIC18F46K42"
  uint8_t command_input[14] = { 0x01 };
  uint8_t expected_output[14] = { 0x00, 0x05, mcu_size };
  strncpy(expected_output + 3, mcu, mcu_size);

  uint8_t *actual_output = helper_run_command(&state, PS2PLUS_GV_COMMAND_ID, command_input, payload_size);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, payload_size);
}

void test_command_ps2plus_get_version_configuration() {
  const size_t payload_size = 4;

  // Request configuration version (02h) and expect version 1 (0001h)
  uint8_t command_input[4] = { 0x02 };
  uint8_t expected_output[4] = { 0x00, 0x03, 0x01, 0x00 };
  uint8_t *actual_output = helper_run_command(&state, PS2PLUS_GV_COMMAND_ID, command_input, payload_size);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, payload_size);
}