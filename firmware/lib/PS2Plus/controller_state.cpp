#include "controller_state.h"

void controller_state_initialize(controller_state *state) {
  controller_input_initialize(&state->input);
  state->analog_mode = CMDigital;
  state->analog_mode_locked = false;
  state->config_mode = false;
  state->rumble_motor_small.mapping = 0xFF;
  state->rumble_motor_small.value = 0x00;
  state->rumble_motor_large.mapping = 0xFF;
  state->rumble_motor_large.value = 0x00;
}

uint8_t controller_state_get_mode(controller_state *state) {
  if (state->config_mode) {
    return 0xF3;
  }

  return state->analog_mode;
}

bool controller_state_is_digital(controller_state *state) {
  return state->analog_mode == CMDigital;
}

bool controller_state_is_analog(controller_state *state) {
  return state->analog_mode != CMDigital;
}

bool controller_state_include_button_pressure(controller_state *state) {
  return state->analog_mode == CMAnalogFull;
}