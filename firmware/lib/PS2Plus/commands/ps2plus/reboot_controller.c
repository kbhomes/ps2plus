#ifdef PS2PLUS_FIRMWARE

#include "../command.h"

command_result rc_initialize(volatile command_packet *packet, controller_state *state) {
  return CRInitialized;
}

command_result rc_process(volatile command_packet *packet, controller_state *state) {
  // Reboot Controller command has no payload, so no response is written
  return CRCompleted;
}

command_result rc_finalize(volatile command_packet *packet, controller_state *state) {
  // Reset the device execution
  platform_reset();
  return CRCompleted;
}

command_processor command_ps2plus_reboot_controller = {
    .id = 0x7D,
    .initialize = &rc_initialize,
    .process = &rc_process,
    .finalize = &rc_finalize,
};

#endif