#include "test_commands.h"

const uint8_t COMMAND_ID = 0x43;

const size_t PAYLOAD_ANALOG_FULL_SIZE = 18;
const uint8_t PAYLOAD_EXIT_CONFIG_BYTES[PAYLOAD_ANALOG_FULL_SIZE] = { 0x00 };
const uint8_t PAYLOAD_ENTER_CONFIG_BYTES[PAYLOAD_ANALOG_FULL_SIZE] = { 0x01 };

void test_enter_exit_config_command_digital_mode_polling() {
  state.analog_mode = CMDigital;
  poll_check_digital_buttons(COMMAND_ID, PAYLOAD_ENTER_CONFIG_BYTES);
}

void test_enter_exit_config_command_analog_mode_polling() {
  state.analog_mode = CMAnalog;
  poll_check_digital_buttons(COMMAND_ID, PAYLOAD_ENTER_CONFIG_BYTES);
  poll_check_analog_joysticks(COMMAND_ID, PAYLOAD_ENTER_CONFIG_BYTES);
}

void test_enter_exit_config_command_analog_full_mode_polling() {
  state.analog_mode = CMAnalogFull;
  poll_check_digital_buttons(COMMAND_ID, PAYLOAD_ENTER_CONFIG_BYTES);
  poll_check_analog_joysticks(COMMAND_ID, PAYLOAD_ENTER_CONFIG_BYTES);
  poll_check_analog_pressures(COMMAND_ID, PAYLOAD_ENTER_CONFIG_BYTES);
}

void test_enter_exit_config_command_enter_config() {
  state.config_mode = false;

  helper_run_command(&state, COMMAND_ID, PAYLOAD_ENTER_CONFIG_BYTES, poll_payload_size_for_mode(state.analog_mode));
  TEST_ASSERT_TRUE_MESSAGE(state.config_mode, "Controller should be in config mode");

  helper_run_command(&state, COMMAND_ID, PAYLOAD_ENTER_CONFIG_BYTES, poll_payload_size_for_mode(state.analog_mode));
  TEST_ASSERT_TRUE_MESSAGE(state.config_mode, "Controller should remain in config mode");
}

void test_enter_exit_config_command_exit_config() {
  state.config_mode = true;

  helper_run_command(&state, COMMAND_ID, PAYLOAD_EXIT_CONFIG_BYTES, poll_payload_size_for_mode(state.analog_mode));
  TEST_ASSERT_FALSE_MESSAGE(state.config_mode, "Controller should be in non-config mode");

  helper_run_command(&state, COMMAND_ID, PAYLOAD_EXIT_CONFIG_BYTES, poll_payload_size_for_mode(state.analog_mode));
  TEST_ASSERT_FALSE_MESSAGE(state.config_mode, "Controller should remain in non-config mode");
}