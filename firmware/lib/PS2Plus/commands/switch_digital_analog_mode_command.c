#include "command.h"

command_result sdam_initialize(command_packet *packet, controller_state *state) {
  // No initialization or memory state management needed
  return CRInitialized;
}

command_result sdam_process(command_packet *packet, controller_state *state) {
  // Response appears to always be a 0x00
  packet->write(0);

  // Determine which mode to set the controller to
  if (packet->command_index == 0) {
    state->analog_mode = (packet->command_byte == 0x01) ? CMAnalog : CMDigital;
  }

  // Determine whether analog mode is locked by the console
  if (packet->command_index == 1) {
    state->analog_mode_locked = packet->command_byte == 0x03;
  }

  // If the final byte hasn't been written, mark this command as still processing
  if (packet->data_index + 1 != 6) {
    return CRProcessing;
  }

  return CRCompleted;
}

command_processor switch_digital_analog_mode_command = {
    .id = 0x44,
    .initialize = &sdam_initialize,
    .process = &sdam_process,
};