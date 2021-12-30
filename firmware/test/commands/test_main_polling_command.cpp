#include "test_commands.h"

const uint8_t COMMAND_ID = 0x42;

const uint8_t PAYLOAD_BYTES[POLLING_PAYLOAD_ANALOG_FULL_SIZE] = { 0x00 };
const uint8_t PAYLOAD_MOTOR_CONTROL_BYTES[POLLING_PAYLOAD_ANALOG_FULL_SIZE] = { 0xFF, 0x40 };

void test_main_polling_command_digital_mode() {
  state.analog_mode = CMDigital;
  polling_check_digital_buttons(&state, COMMAND_ID, PAYLOAD_BYTES);
}

void test_main_polling_command_analog_mode() {
  state.analog_mode = CMAnalog;
  polling_check_digital_buttons(&state, COMMAND_ID, PAYLOAD_BYTES);
  polling_check_analog_joysticks(&state, COMMAND_ID, PAYLOAD_BYTES);
}

void test_main_polling_command_analog_full_mode() {
  state.analog_mode = CMAnalogFull;
  polling_check_digital_buttons(&state, COMMAND_ID, PAYLOAD_BYTES);
  polling_check_analog_joysticks(&state, COMMAND_ID, PAYLOAD_BYTES);
  polling_check_analog_pressures(&state, COMMAND_ID, PAYLOAD_BYTES);
}

void test_main_polling_command_with_motors() {
  state.analog_mode = CMAnalogFull;
  state.rumble_motor_small.mapping = 0x00;
  state.rumble_motor_large.mapping = 0x01;

  // Ensure the controller polling still works if the motors are being controlled
  polling_check_digital_buttons(&state, COMMAND_ID, PAYLOAD_MOTOR_CONTROL_BYTES);
  polling_check_analog_joysticks(&state, COMMAND_ID, PAYLOAD_MOTOR_CONTROL_BYTES);
  polling_check_analog_pressures(&state, COMMAND_ID, PAYLOAD_MOTOR_CONTROL_BYTES);

  TEST_ASSERT_EQUAL_HEX8_MESSAGE(state.rumble_motor_small.value, PAYLOAD_MOTOR_CONTROL_BYTES[0], "Small motor should be controlled");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(state.rumble_motor_large.value, PAYLOAD_MOTOR_CONTROL_BYTES[1], "Large motor should be controlled");
}