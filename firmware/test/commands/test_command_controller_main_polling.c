#include "test_commands.h"

#define MP_COMMAND_ID 0x42

const uint8_t MP_PAYLOAD_BYTES[POLLING_PAYLOAD_ANALOG_FULL_SIZE] = { 0x00 };
const uint8_t MP_PAYLOAD_MOTOR_CONTROL_BYTES[POLLING_PAYLOAD_ANALOG_FULL_SIZE] = { 0xFF, 0x40 };

void test_command_controller_main_polling_digital_mode() {
  state.analog_mode = CMDigital;
  polling_check_digital_buttons(&state, MP_COMMAND_ID, MP_PAYLOAD_BYTES);
}

void test_command_controller_main_polling_analog_mode() {
  state.analog_mode = CMAnalog;
  polling_check_digital_buttons(&state, MP_COMMAND_ID, MP_PAYLOAD_BYTES);
  polling_check_analog_joysticks(&state, MP_COMMAND_ID, MP_PAYLOAD_BYTES);
}

void test_command_controller_main_polling_analog_full_mode() {
  state.analog_mode = CMAnalogFull;
  polling_check_digital_buttons(&state, MP_COMMAND_ID, MP_PAYLOAD_BYTES);
  polling_check_analog_joysticks(&state, MP_COMMAND_ID, MP_PAYLOAD_BYTES);
  polling_check_analog_pressures(&state, MP_COMMAND_ID, MP_PAYLOAD_BYTES);
}

void test_command_controller_main_polling_with_motors() {
  state.analog_mode = CMAnalogFull;
  state.rumble_motor_small.mapping = 0x00;
  state.rumble_motor_large.mapping = 0x01;

  // Ensure the controller polling still works if the motors are being controlled
  polling_check_digital_buttons(&state, MP_COMMAND_ID, MP_PAYLOAD_MOTOR_CONTROL_BYTES);
  polling_check_analog_joysticks(&state, MP_COMMAND_ID, MP_PAYLOAD_MOTOR_CONTROL_BYTES);
  polling_check_analog_pressures(&state, MP_COMMAND_ID, MP_PAYLOAD_MOTOR_CONTROL_BYTES);

  TEST_ASSERT_EQUAL_HEX8_MESSAGE(state.rumble_motor_small.value, MP_PAYLOAD_MOTOR_CONTROL_BYTES[0], "Small motor should be controlled");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(state.rumble_motor_large.value, MP_PAYLOAD_MOTOR_CONTROL_BYTES[1], "Large motor should be controlled");
}