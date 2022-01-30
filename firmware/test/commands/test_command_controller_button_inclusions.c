#include "test_commands.h"

#define BI_COMMAND_ID 0x41
#define BI_PAYLOAD_SIZE 6

const uint8_t BI_COMMAND_INPUT[BI_PAYLOAD_SIZE] = { 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A };
const uint8_t BI_EXPECTED_DIGITAL_OUTPUT[BI_PAYLOAD_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const uint8_t BI_EXPECTED_ANALOG_OUTPUT[BI_PAYLOAD_SIZE] = { 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x5A };

void test_command_controller_button_inclusions_digital_mode() {
  state.analog_mode = CMDigital;

  uint8_t *actual_output = helper_run_command(&state, BI_COMMAND_ID, BI_COMMAND_INPUT, BI_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(BI_EXPECTED_DIGITAL_OUTPUT, actual_output, BI_PAYLOAD_SIZE);
}

void test_command_controller_button_inclusions_analog_mode() {
  state.analog_mode = CMAnalog;

  uint8_t *actual_output = helper_run_command(&state, BI_COMMAND_ID, BI_COMMAND_INPUT, BI_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(BI_EXPECTED_ANALOG_OUTPUT, actual_output, BI_PAYLOAD_SIZE);
}

void test_command_controller_button_inclusions_analog_full_mode() {
  state.analog_mode = CMAnalogFull;

  uint8_t *actual_output = helper_run_command(&state, BI_COMMAND_ID, BI_COMMAND_INPUT, BI_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(BI_EXPECTED_ANALOG_OUTPUT, actual_output, BI_PAYLOAD_SIZE);
}