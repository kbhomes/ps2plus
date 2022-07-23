#include "suite.h"

#include <string.h>

#define PS2PLUS_GV_COMMAND_ID 0x70

void test_command_ps2plus_get_version_firmware() {
  // Request firmware version (00h)
  uint8_t command_input[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t expected_output[] = { 0x00, PDT_Uint64, 0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01 };
  uint8_t *actual_output = helper_run_command(&state, command_ps2plus_get_version.id, command_input, sizeof(command_input));
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, sizeof(command_input));
}

void test_command_ps2plus_get_version_microcontroller() {
  const size_t mcu_size = strlen(TEST_VERSION_MICROCONTROLLER);
  const size_t payload_size = 3 + mcu_size;

  // Request microcontroller version (01h)
  uint8_t command_input[128] = { 0x01 };
  uint8_t expected_output[128] = { 0x00, PDT_Array, mcu_size };
  strncpy(expected_output + 3, TEST_VERSION_MICROCONTROLLER, mcu_size);

  uint8_t *actual_output = helper_run_command(&state, command_ps2plus_get_version.id, command_input, payload_size);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, payload_size);
}

void test_command_ps2plus_get_version_configuration() {
  const size_t payload_size = 4;

  // Request configuration version (02h)
  uint8_t command_input[4] = { 0x02 };
  uint8_t expected_output[4] = { 0x00, PDT_Uint16, 0xCD, 0xAB };
  uint8_t *actual_output = helper_run_command(&state, command_ps2plus_get_version.id, command_input, payload_size);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, payload_size);
}