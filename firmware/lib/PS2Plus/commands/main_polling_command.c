#include "command.h"
#include <stdio.h>

const uint8_t CONFIG_MODE_RESPONSE_BYTES[6] = { 0x00 };

struct {
  const uint8_t *response_data;
  size_t response_length;
} mpc_memory;

command_result mpc_initialize(command_packet *packet, controller_state *state) {
  // Determines how much data will be sent back to the console depending on controller mode
  if (state->config_mode) {
    // In config mode, all zeroes are written
    mpc_memory.response_data = CONFIG_MODE_RESPONSE_BYTES;
    mpc_memory.response_length = 6;
  } else {
    // For standard polling, simply write the controller data to the console one byte at a time
    mpc_memory.response_data = state->input.button_data;

    if (state->analog_mode == CMDigital) {
      mpc_memory.response_length = 2;
    } else if (state->analog_mode == CMAnalog) {
      mpc_memory.response_length = 6;
    } else if (state->analog_mode == CMAnalogFull) {
      mpc_memory.response_length = 18;
    } else {
      mpc_memory.response_length = 0;
    }
  }

  return CRInitialized;
}

command_result mpc_process(command_packet *packet, controller_state *state) {
  packet->write(mpc_memory.response_data[packet->data_index]);
  
  if (packet->id == 0x42) {
    // Attempt to power the small/large motors, if necessary
    if (packet->command_index == 0 && state->rumble_motor_small.mapping == 0x00) {
      state->rumble_motor_small.value = (packet->command_byte == 0xFF) ? 0xFF : 0x00;
    }

    if (packet->command_index == 1 && state->rumble_motor_large.mapping == 0x01) {
      state->rumble_motor_large.value = packet->command_byte;
    }
  } else if (packet->id == 0x43) {
    if (packet->command_index == 0) {
      state->config_mode = packet->command_byte == 0x01;
    }
  }

  // If the final byte hasn't been written, mark this command as still processing
  if (packet->data_index + 1 != mpc_memory.response_length) {
    return CRProcessing;
  }

  return CRCompleted;
}

command_processor main_polling_command = { 
  .id = 0x42,
  .initialize = &mpc_initialize,
  .process = &mpc_process,
};

// Since the polling behavior of the 43h command is nearly identical to the 42h command (main polling), 
// its processing is implemented as part of the main polling command. Whether to actuate the rumble 
// motors (as in 42h) or enter/exit config mode (as in 43h) is differented based on the command ID.
command_processor enter_exit_config_command = {
  .id = 0x43,
  .initialize = &mpc_initialize,
  .process = &mpc_process,
};