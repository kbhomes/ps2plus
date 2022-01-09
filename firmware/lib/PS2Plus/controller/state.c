#include "state.h"

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

void controller_state_update_mode(controller_state *state) {
  if (state->config_mode) {
    state->mode = 0xF3;
  } else if (state->analog_mode == CMDigital) {
    state->mode = 0x41;
  } else if (state->analog_mode == CMAnalog) {
    state->mode = 0x73;
  } else if (state->analog_mode == CMAnalogFull) {
    state->mode = 0x79;
  }
}