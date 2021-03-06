#include "suite.h"

#define EEC_COMMAND_ID 0x43

const uint8_t EEC_PAYLOAD_EXIT_CONFIG_BYTES[POLLING_PAYLOAD_ANALOG_FULL_SIZE] = { 0x00 };
const uint8_t EEC_PAYLOAD_ENTER_CONFIG_BYTES[POLLING_PAYLOAD_ANALOG_FULL_SIZE] = { 0x01 };

void test_command_controller_enter_exit_config_digital_mode_polling() {
  state.analog_mode = CMDigital;
  polling_check_digital_buttons(EEC_COMMAND_ID, EEC_PAYLOAD_EXIT_CONFIG_BYTES);
}

void test_command_controller_enter_exit_config_analog_mode_polling() {
  state.analog_mode = CMAnalog;
  polling_check_digital_buttons(EEC_COMMAND_ID, EEC_PAYLOAD_EXIT_CONFIG_BYTES);
  polling_check_analog_joysticks(EEC_COMMAND_ID, EEC_PAYLOAD_EXIT_CONFIG_BYTES);
}

void test_command_controller_enter_exit_config_analog_full_mode_polling() {
  state.analog_mode = CMAnalogFull;
  polling_check_digital_buttons(EEC_COMMAND_ID, EEC_PAYLOAD_EXIT_CONFIG_BYTES);
  polling_check_analog_joysticks(EEC_COMMAND_ID, EEC_PAYLOAD_EXIT_CONFIG_BYTES);
  polling_check_analog_pressures(EEC_COMMAND_ID, EEC_PAYLOAD_EXIT_CONFIG_BYTES);
}

void test_command_controller_enter_exit_config_enter_config() {
  state.config_mode = false;

  helper_run_command(EEC_COMMAND_ID, EEC_PAYLOAD_ENTER_CONFIG_BYTES, polling_payload_size_for_mode(state.config_mode, state.analog_mode));
  TEST_ASSERT_TRUE_MESSAGE(state.config_mode, "Controller should be in config mode");

  helper_run_command(EEC_COMMAND_ID, EEC_PAYLOAD_ENTER_CONFIG_BYTES, polling_payload_size_for_mode(state.config_mode, state.analog_mode));
  TEST_ASSERT_TRUE_MESSAGE(state.config_mode, "Controller should remain in config mode");
}

void test_command_controller_enter_exit_config_exit_config() {
  state.config_mode = true;

  helper_run_command(EEC_COMMAND_ID, EEC_PAYLOAD_EXIT_CONFIG_BYTES, polling_payload_size_for_mode(state.config_mode, state.analog_mode));
  TEST_ASSERT_FALSE_MESSAGE(state.config_mode, "Controller should be in non-config mode");

  helper_run_command(EEC_COMMAND_ID, EEC_PAYLOAD_EXIT_CONFIG_BYTES, polling_payload_size_for_mode(state.config_mode, state.analog_mode));
  TEST_ASSERT_FALSE_MESSAGE(state.config_mode, "Controller should remain in non-config mode");
}