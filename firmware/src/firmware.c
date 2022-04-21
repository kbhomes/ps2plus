#ifdef PS2PLUS_FIRMWARE

#include "main.h"

void check_configuration_version(volatile controller_state *state) {
  // If the stored configuration version does not match this firmware's version, reset
  // all custom configuration and save it back to storage
  if (state->custom_config.configuration_version != VERSION_CONFIGURATION) {
    puts("[firmware] Resetting config due to mismatching versions");
    controller_custom_config_initialize(&state->custom_config);
    controller_custom_config_save(&state->custom_config);
  }
}

void debug_versions_dump(volatile controller_state *state) {
  printf("[version] Firmware = %u\n", VERSION_FIRMWARE);
  printf("[version] Microcontroller = %s\n", VERSION_MICROCONTROLLER);
  printf("[version] Configuration = %u\n", VERSION_CONFIGURATION);
}

void main_init(volatile controller_state *state) {
  puts("[firmware] Initializing");
  debug_versions_dump(state);
  
  // Load the configuration from storage and check the configuration version
  controller_custom_config_load(&state->custom_config);
  check_configuration_version(state);
}

void main_loop(volatile controller_state *state) {
  
}

#endif