#include "../command.h"

command_result rcd_initialize(volatile command_packet *packet, controller_state *state) {
  controller_custom_config_initialize(&state->custom_config);
  controller_custom_config_save(&state->custom_config);

  return CRInitialized;
}

command_result rcd_process(volatile command_packet *packet, controller_state *state) {
  // Restore Configuration Defaults command has no payload, so no response is written
  return CRCompleted;
}

command_processor command_ps2plus_restore_configuration_defaults = {
    .id = 0x74,
    .initialize = &rcd_initialize,
    .process = &rcd_process,
};