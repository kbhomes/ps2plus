#include "command.h"

command_result mm_initialize(command_packet *packet, controller_state *state) {
  // No initialization or memory state management needed
  return CRInitialized;
}

command_result mm_process(command_packet *packet, controller_state *state) {
  // TODO: Documentation about this making an (unfounded?) assumption that
  //       the first/second bytes will always be the small/large motors. As
  //       far as I can see from online documentation, the small/large motor
  //       mappings could come in any order or position in the packet.

  // Write the current motor mappings 
  if (packet->data_index == 0) {
    packet->write(state->rumble_motor_small.mapping);
  } else if (packet->data_index == 1) {
    packet->write(state->rumble_motor_large.mapping);
  } else {
    packet->write(0xFF);
  }

  // Read the new motor mappings
  if (packet->command_index == 0) {
    state->rumble_motor_small.mapping = packet->command_byte;
  } else if (packet->command_index == 1) {
    state->rumble_motor_large.mapping = packet->command_byte;
  }
 
  // If the final byte hasn't been written, mark this command as still processing
  if (packet->data_index + 1 != 6) {
    return CRProcessing;
  }

  return CRCompleted;
}

command_processor map_motors_command = {
    .id = 0x4D,
    .initialize = &mm_initialize,
    .process = &mm_process,
};