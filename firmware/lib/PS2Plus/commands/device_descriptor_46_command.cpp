#include "command.h"

const uint8_t DD46_CONSTANT_BYTES[2][5] = { 
  { 0x00, 0x01, 0x02, 0x00, 0x0A },
  { 0x00, 0x01, 0x01, 0x01, 0x14 },
};

struct {
  int constant_index;
} dd46_memory;

command_result dd46_initialize(controller_state *state) {
  dd46_memory.constant_index = 0;

  return CRInitialized;
}

command_result dd46_process(command_packet *packet, controller_state *state) {
  if (packet->data_index == 0) {
    packet->write(0x00);
  } else {
    if (packet->command_index == 0) {
      // Determines which byte array should be sent as part of this command's response
      // (only the bottom bit of this byte is taken to avoid going out-of-bounds)
      dd46_memory.constant_index = packet->command_byte & 0b1;
    }

    // Write the corresponding byte from the selected constant
    const uint8_t *target_bytes = DD46_CONSTANT_BYTES[dd46_memory.constant_index];
    packet->write(target_bytes[packet->data_index - 1]);
  }

  // If the final byte hasn't been written, mark this command as still processing
  if (packet->data_index + 1 != 6) {
    return CRProcessing;
  }

  return CRCompleted;
}

command_processor device_descriptor_46_command = {
    .id = 0x46,
    .initialize = &dd46_initialize,
    .process = &dd46_process,
};