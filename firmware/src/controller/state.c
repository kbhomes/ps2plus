#include "state.h"

#include <string.h>

void controller_state_initialize(controller_state *state) {
  controller_state_reset(&state);
  
#if defined(PS2PLUS_FIRMWARE)
  controller_input_initialize(&state->input);
  controller_custom_config_initialize(&state->custom_config);
#elif defined(PS2PLUS_BOOTLOADER)
  state->bootloader.status = BLStatusPending;
#endif
}

void controller_state_update_mode(controller_state *state) {
#if defined(PS2PLUS_FIRMWARE)
  if (state->config_mode) {
    state->mode = 0xF3;
  } else if (state->analog_mode == CMDigital) {
    state->mode = 0x41;
  } else if (state->analog_mode == CMAnalog) {
    state->mode = 0x73;
  } else if (state->analog_mode == CMAnalogFull) {
    state->mode = 0x79;
  }
#elif defined(PS2PLUS_BOOTLOADER)
  state->mode = 0xBB;
#endif
}

void controller_state_set_versions(controller_state *state, uint16_t firmware, const char microcontroller[32], uint16_t configuration, uint16_t bootloader) {
#if defined(PS2PLUS_FIRMWARE)
  primitive_data_initialize_uint16(&state->versions.firmware, firmware);
  primitive_data_initialize_array(&state->versions.microcontroller, microcontroller, strlen(microcontroller));
  primitive_data_initialize_uint16(&state->versions.configuration, configuration);
#elif defined (PS2PLUS_BOOTLOADER)
  primitive_data_initialize_uint16(&state->versions.bootloader, bootloader);
#endif
}

void controller_state_reset(controller_state *state) {
#if defined(PS2PLUS_FIRMWARE)
  state->mode = 0x41;
  state->analog_mode = CMDigital;
  state->analog_mode_locked = false;
  state->config_mode = false;
  state->rumble_motor_small.mapping = 0xFF;
  state->rumble_motor_small.value = 0x00;
  state->rumble_motor_large.mapping = 0xFF;
  state->rumble_motor_large.value = 0x00;
#elif defined(PS2PLUS_BOOTLOADER)
  state->mode = 0xBB;
#endif
  
  state->last_communication_time = UINT64_MAX;
}