#include "command.h"

command_result mm_initialize(controller_state *state) {
  // No initialization or memory state management needed
  return CRInitialized;
}

command_result mm_process(command_packet *packet, controller_state *state) {
  // TODO: Documentation about this making an (unfounded?) assumption that
  //       the first/second bytes will always be the small/large motors. As
  //       far as I can see from online documentation, the small/large motor
  //       mappings could come in any order or position in the packet.

  // Write the current motor mappings 
  if (packet->index == 0) {
    platform_spi_write(state->rumble_motor_small.mapping);
  } else if (packet->index == 1) {
    platform_spi_write(state->rumble_motor_large.mapping);
  } else {
    platform_spi_write(0xFF);
  }

  // Read the new motor mappings
  if (packet->index == 1) {
    state->rumble_motor_small.mapping = packet->value;
  } else if (packet->index == 2) {
    state->rumble_motor_large.mapping = packet->value;
  }
 
  // If the final byte hasn't been written, mark this command as still processing
  if (packet->index + 1 != 6) {
    return CRProcessing;
  }

  return CRCompleted;
}

command_processor button_inclusions_command = {
    .id = 0x4D,
    .initialize = &mm_initialize,
    .process = &mm_process,
};