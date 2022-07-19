#ifdef PS2PLUS_FIRMWARE

#include "../command.h"

command_result rc_initialize(volatile command_packet *packet, controller_state *state) {
  return CRInitialized;
}

command_result rc_process(volatile command_packet *packet, controller_state *state) {
  // Reset the device execution
  platform_reset();
  return CRCompleted;
}

const command_processor command_ps2plus_reboot_controller = {
    .id = 0x7D,
    .initialize = &rc_initialize,
    .process = &rc_process,
};

#endif