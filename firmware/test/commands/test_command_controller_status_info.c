#include "test_commands.h"

#define SI_COMMAND_ID 0x45
#define SI_PAYLOAD_SIZE 6

const uint8_t SI_COMMAND_INPUT[SI_PAYLOAD_SIZE] = { 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, };

void test_command_controller_status_info_digital_mode() {
  state.analog_mode = CMDigital;

  uint8_t expected_output[SI_PAYLOAD_SIZE] = { 0x03, 0x02, 0x00, 0x02, 0x01, 0x00, };
  uint8_t *actual_output = helper_run_command(&state, SI_COMMAND_ID, SI_COMMAND_INPUT, SI_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, SI_PAYLOAD_SIZE);
}

void test_command_controller_status_info_analog_mode() {
  state.analog_mode = CMAnalog;

  uint8_t expected_output[SI_PAYLOAD_SIZE] = { 0x03, 0x02, 0x01, 0x02, 0x01, 0x00, };
  uint8_t *actual_output = helper_run_command(&state, SI_COMMAND_ID, SI_COMMAND_INPUT, SI_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, SI_PAYLOAD_SIZE);
}

void test_command_controller_status_info_analog_full_mode() {
  state.analog_mode = CMAnalogFull;

  uint8_t expected_output[SI_PAYLOAD_SIZE] = { 0x03, 0x02, 0x01, 0x02, 0x01, 0x00, };
  uint8_t *actual_output = helper_run_command(&state, SI_COMMAND_ID, SI_COMMAND_INPUT, SI_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, SI_PAYLOAD_SIZE);
}