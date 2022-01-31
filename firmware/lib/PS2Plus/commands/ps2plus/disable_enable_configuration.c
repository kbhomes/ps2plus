#include "../command.h"

command_result dec_initialize(volatile command_packet *packet, controller_state *state) {
  return CRInitialized;
}

command_result dec_process(volatile command_packet *packet, controller_state *state) {
  // Write the current status
  packet->write(state->custom_config.enabled ? 0xFF : 0x00);
  return CRCompleted;
}

void dec_finalize(volatile command_packet *packet, controller_state *state) {
  // Save the new status
  state->custom_config.enabled = (packet->command_byte != 0x00);
}

command_processor command_ps2plus_disable_enable_configuration = {
    .id = 0x73,
    .initialize = &dec_initialize,
    .process = &dec_process,
    .finalize = &dec_finalize,
};