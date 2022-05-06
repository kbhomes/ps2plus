#ifdef PS2PLUS_BOOTLOADER

#include "../command.h"

command_result bufd_initialize(volatile command_packet *packet, controller_state *state) {
  state->bootloader.status = BLStatusPending;
  state->bootloader.update.ready = false;
  state->bootloader.update.record.target_address = 0;
  state->bootloader.update.record.data_length = 0xFF;
  for (size_t i = 0; i < sizeof(state->bootloader.update.record.data); i++) {
    state->bootloader.update.record.data[i] = 0;
  }
  
  return CRInitialized;
}

command_result bufd_process(volatile command_packet *packet, controller_state *state) {
  packet->write(packet->data_index);
  
  if (packet->command_index == 0) {
    state->bootloader.update.record.type = packet->command_byte;
  } else if (packet->command_index == 1) {
    state->bootloader.update.record.target_address |= ((uint32_t)packet->command_byte);
  } else if (packet->command_index == 2) {
    state->bootloader.update.record.target_address |= ((uint32_t)packet->command_byte) << 8;
  } else if (packet->command_index == 3) {
    state->bootloader.update.record.target_address |= ((uint32_t)packet->command_byte) << 16;
  } else if (packet->command_index == 4) {
    state->bootloader.update.record.target_address |= ((uint32_t)packet->command_byte) << 24;
  } else if (packet->command_index == 5) {
    state->bootloader.update.record.data_length = packet->command_byte;
  } else if (packet->command_index == 6 + state->bootloader.update.record.data_length) {
    state->bootloader.update.record.data_checksum = packet->command_byte;
  } else {
    state->bootloader.update.record.data[packet->command_index - 6] = packet->command_byte;
  }

  // If the final byte hasn't been written, mark this command as still processing
  if (packet->data_index + 1 != 6 + state->bootloader.update.record.data_length + 1) {
    return CRProcessing;
  }

  return CRCompleted;
}

command_result bufd_finalize(volatile command_packet *packet, controller_state *state) {
  state->bootloader.update.record.data_checksum = packet->command_byte;
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