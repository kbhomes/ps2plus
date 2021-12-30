#include "controller_state.h"

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