#include "../command.h"

const uint8_t BI_ANALOG_INCLUSION_BYTES[6] = { 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x5A };

command_result bi_initialize(volatile command_packet *packet, controller_state *state) {
  // No initialization or memory state management needed
  return CRInitialized;
}

command_result bi_process(volatile command_packet *packet, controller_state *state) {
  // Select the correct response based on digital/analog status
  if (state->analog_mode == CMDigital) {
    packet->write(0);
  } else {
    packet->write(BI_ANALOG_INCLUSION_BYTES[packet->data_index]);
  }

  // If the final byte hasn't been written, mark this command as still processing
  if (packet->data_index + 1 != 6) {
    return CRProcessing;
  }

  return CRCompleted;
}

command_processor command_controller_button_inclusions = {
    .id = 0x41,
    .initialize = &bi_initialize,
    .process = &bi_process,
};