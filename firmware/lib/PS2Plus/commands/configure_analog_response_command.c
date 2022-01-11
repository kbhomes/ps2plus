#include "packet.h"

const uint8_t CAR_RESPONSE_BYTES[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A };

command_result car_initialize(command_packet *packet, controller_state *state) {
  // No initialization or memory state management needed
  return CRInitialized;
}

command_result car_process(command_packet *packet, controller_state *state) {
  packet->write(CAR_RESPONSE_BYTES[packet->data_index]);
  
  // Apparently there is no indication that any software actually does configuration
  // of individual pressure-sensitive buttons; so we'll just set the controller into
  // full analog mode for all buttons regardless of the command data.
  if (state->analog_mode == CMAnalog) {
    state->analog_mode = CMAnalogFull;
  }

  // If the final byte hasn't been written, mark this command as still processing
  if (packet->data_index + 1 != 6) {
    return CRProcessing;
  }

  return CRCompleted;
}

command_processor configure_analog_response_command = {
    .id = 0x4F,
    .initialize = &car_initialize,
    .process = &car_process,
};