#ifdef PS2PLUS_BOOTLOADER

#include "../command.h"

struct {
  uint8_t response[3];
} bqfus_memory;

command_result bqfus_initialize(volatile command_packet *packet, controller_state *state) {
  bqfus_memory.response[0] = state->bootloader.status;
    
  if (state->bootloader.status == BLStatusError) {
    // Serialize the error state, LSB first
    bqfus_memory.response[1] = state->bootloader.error & 0xFF;
    bqfus_memory.response[2] = (state->bootloader.error >> 8) & 0xFF;
  } else {
    // No additional data sent for non-error states
    bqfus_memory.response[1] = 0;
    bqfus_memory.response[2] = 0;
  }
  
  return CRInitialized;
}

command_result bqfus_process(volatile command_packet *packet, controller_state *state) {
  packet->write(bqfus_memory.response[packet->data_index]);

  // If the final byte hasn't been written, mark this command as still processing
  if (packet->data_index + 1 != 3) {
    return CRProcessing;
  }

  return CRCompleted;
}

command_processor command_ps2plus_bootloader_query_firmware_update_status = {
    .id = 0x7F,
    .initialize = &bqfus_initialize,
    .process = &bqfus_process,
};

#endif