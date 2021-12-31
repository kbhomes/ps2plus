#include "polling_helper.h"
#include "command_helper.h"

size_t polling_payload_size_for_mode(controller_analog_mode mode) {
  switch (mode) {
    case CMDigital: 
      return POLLING_PAYLOAD_DIGITAL_SIZE;

    case CMAnalog: 
      return POLLING_PAYLOAD_ANALOG_SIZE;

    case CMAnalogFull: 
      return POLLING_PAYLOAD_ANALOG_FULL_SIZE;

    default: return 0;
  }
}

void polling_check_digital_buttons(controller_state *state, uint8_t command_id, const uint8_t *payload) {
  const size_t payload_size = polling_payload_size_for_mode(state->analog_mode);

  // Test the digital output when no buttons are pressed
  controller_input_initialize(&state->input);
  uint8_t *actual_output = helper_run_command(state, command_id, payload, payload_size);
  TEST_ASSERT_EACH_EQUAL_HEX8_MESSAGE(0x00, actual_output, POLLING_PAYLOAD_DIGITAL_SIZE, "Digital button output should be zeros when no button is pressed");

  // Test every digital button individually
  for (int i = 0; i < NUM_DIGITAL_BUTTONS; i++) {
    // Reset the command output buffer in each iteration, since the output
    // of the previous iteration isn't needed in the current one
    helper_reset_output();

    // Set the specific button and clear the others
    controller_input_initialize(&state->input);
    debounced_force(&state->input.digital_buttons[i], true);
    uint16_t expected_output = controller_input_as_digital(&state->input);
    uint8_t *actual_output = helper_run_command(state, command_id, payload, payload_size);

    char message_buffer[64];
    sprintf(message_buffer, "Only digital button %d should be pressed", i);
    TEST_ASSERT_EQUAL_HEX16_MESSAGE(expected_output, (actual_output[0] << 8) | actual_output[1], message_buffer);
  }
}

void polling_check_analog_joysticks(controller_state *state, uint8_t command_id, const uint8_t *payload) {
  const size_t payload_size = polling_payload_size_for_mode(state->analog_mode);

  // Generate random joystick values
  controller_input_initialize(&state->input);
  for (size_t i = 0; i < NUM_JOYSTICK_AXES; i++) {
    state->input.joysticks[i] = rand() & 0xFF;
  }

  // Ensure the output contains these joystick values
  uint8_t *actual_output = helper_run_command(state, command_id, payload, payload_size);
  for (size_t i = 0; i < NUM_JOYSTICK_AXES; i++) {
    char message_buffer[64];
    sprintf(message_buffer, "Joystick axis %zu should have value %02X", i, state->input.joysticks[i]);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(state->input.joysticks[i], actual_output[POLLING_PAYLOAD_DIGITAL_SIZE + i], message_buffer);
  }
}

void polling_check_analog_pressures(controller_state *state, uint8_t command_id, const uint8_t *payload) {
  const size_t payload_size = polling_payload_size_for_mode(state->analog_mode);
  
  // Test the analog pressure output when no buttons are pressed
  controller_input_initialize(&state->input);
  uint8_t *actual_output = helper_run_command(state, command_id, payload, payload_size);
  uint8_t *analog_pressure_output = &actual_output[POLLING_PAYLOAD_ANALOG_SIZE];
  size_t analog_pressure_size = POLLING_PAYLOAD_ANALOG_FULL_SIZE - POLLING_PAYLOAD_ANALOG_SIZE;
  TEST_ASSERT_EACH_EQUAL_HEX8_MESSAGE(0x00, analog_pressure_output, analog_pressure_size, "Button pressure output should be zeros when no button is pressed");

  // Test every pressure-sensitive button individually
  for (int i = 0; i < NUM_PRESSURE_SENSITIVE_BUTTONS; i++) {
    // Reset the command output buffer in each iteration, since the output
    // of the previous iteration isn't needed in the current one
    helper_reset_output();

    // Set the specific button and clear the others
    controller_input_digital_button button_index = DIGITAL_BUTTON_TO_PRESSURE_INDEX_MAP[i];
    controller_input_initialize(&state->input);
    debounced_force(&state->input.digital_buttons[button_index], true);
    uint8_t *actual_output = helper_run_command(state, command_id, payload, payload_size);

    // Check every button's pressure -- all should be 0x00 except for the current button
    for (int j = 0; j < NUM_PRESSURE_SENSITIVE_BUTTONS; j++) {
      uint8_t expected_pressure = i == j ? 0xFF : 0x00;

      char message_buffer[64];
      sprintf(message_buffer, "Pressure for button %d (%s) should be %02X", j, (i == j) ? "pressed" : "not pressed", expected_pressure);
      TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_pressure, actual_output[POLLING_PAYLOAD_ANALOG_SIZE + j], message_buffer);
    }
  }
}