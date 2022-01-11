#include "packet.h"

const uint8_t SI_RESPONSE_BYTES[6] = { 0x03, 0x02, 0x01, 0x02, 0x01, 0x00 };

command_result si_initialize(command_packet *packet, controller_state *state) {
  // No initialization or memory state management needed
  return CRInitialized;
}

command_result si_process(command_packet *packet, controller_state *state) {
  if (packet->data_index == 2) {
    // Third byte is the status of the analog LED
    packet->write(state->analog_mode == CMDigital ? 0x00 : 0x01);
  } else {
    packet->write(SI_RESPONSE_BYTES[packet->data_index]);
  }

  // If the final byte hasn't been written, mark this command as still processing
  if (packet->data_index + 1 != 6) {
    return CRProcessing;
  }

  return CRCompleted;
}

command_processor status_info_command = {
    .id = 0x45,
    .initialize = &si_initialize,
    .process = &si_process,
};