#ifdef PS2PLUS_BOOTLOADER

#include "../command.h"

command_result bufd_initialize(volatile command_packet *packet, controller_state *state) {
  state->bootloader.status = BLStatusPending;
  state->bootloader.update.ready = false;
  state->bootloader.update.target_address = 0;
  state->bootloader.update.data_length = 0;
  
  return CRInitialized;
}

command_result bufd_process(volatile command_packet *packet, controller_state *state) {
  packet->write(0x01);
  
  if (packet->command_index == 0) {
    state->bootloader.update.record_type = packet->command_byte;
  } else if (packet->command_index == 1) {
    state->bootloader.update.target_address |= packet->command_byte;
  } else if (packet->command_index == 2) {
    state->bootloader.update.target_address |= (packet->command_byte >> 8);
  } else if (packet->command_index == 3) {
    state->bootloader.update.data_length = packet->command_byte;
  } else if (packet->command_index == 3 + state->bootloader.update.data_length + 1) {
    state->bootloader.update.data_checksum = packet->command_byte;
  } else {
    state->bootloader.update.data[packet->command_index - 4] = packet->command_byte;
  }

  // If the final byte hasn't been written, mark this command as still processing
  if (packet->data_index + 1 != 5 + state->bootloader.update.data_length) {
    return CRProcessing;
  }

  return CRCompleted;
}

command_result bufd_finalize(volatile command_packet *packet, controller_state *state) {
  state->bootloader.update.data_checksum = packet->command_byte;
  state->bootloader.update.ready = true;
  
  return CRCompleted;
}

command_processor command_ps2plus_bootloader_update_firmware_data = {
    .id = 0x7E,
    .initialize = &bufd_initialize,
    .process = &bufd_process,
    .finalize = &bufd_finalize,
};

#endif