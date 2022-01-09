#include "command.h"

const uint8_t DD4C_CONSTANT_BYTES[2][5] = { 
  { 0x00, 0x00, 0x04, 0x00, 0x00 },
  { 0x00, 0x00, 0x07, 0x00, 0x10 },
};

struct {
  const uint8_t *constant_bytes;
} dd4c_memory;

command_result dd4c_initialize(controller_state *state) {
  dd4c_memory.constant_bytes = DD4C_CONSTANT_BYTES[0];

  return CRInitialized;
}

command_result dd4c_process(command_packet *packet, controller_state *state) {
  if (packet->data_index == 0) {
    packet->write(0x00);
  } else {
    // Since both constants begin with the same byte, we can optimize by writing first 
    // and ensuring the correct constant is selected afterwards
    packet->write(dd4c_memory.constant_bytes[packet->data_index - 1]);

    if (packet->command_index == 0) {
      // Determines which byte array should be sent as part of this command's response
      dd4c_memory.constant_bytes = DD4C_CONSTANT_BYTES[packet->command_byte & 0x1];
    }
  }

  // If the final byte hasn't been written, mark this command as still processing
  if (packet->data_index + 1 != 6) {
    return CRProcessing;
  }

  return CRCompleted;
}

command_processor device_descriptor_4c_command = {
    .id = 0x4C,
    .initialize = &dd4c_initialize,
    .process = &dd4c_process,
};