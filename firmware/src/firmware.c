#ifdef PS2PLUS_FIRMWARE

#include "main.h"

void check_configuration_version(volatile controller_state *state) {
  // If the stored configuration version does not match this firmware's version, reset
  // all custom configuration and save it back to storage
  if (state->custom_config.configuration_version != state->versions.configuration) {
    controller_custom_config_initialize(&state->custom_config);
    controller_custom_config_save(&state->custom_config);
  }
}

void main_init(volatile controller_state *state) {
  // Load the configuration from storage and check the configuration version
  // controller_custom_config_load(&state->custom_config);
  // check_configuration_version(state);
}

void main_loop(volatile controller_state *state) {
  
}

#endif