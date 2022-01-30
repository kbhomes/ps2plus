#include "../command.h"

const uint8_t CONFIG_MODE_RESPONSE_BYTES[6] = { 0x00 };

struct {
  const uint8_t *response_data;
  size_t response_length;
} memory;

command_result mp_initialize(volatile command_packet *packet, controller_state *state) {
  // Determines how much data will be sent back to the console depending on controller mode
  if (state->config_mode) {
    // In config mode, all zeroes are written
    memory.response_data = CONFIG_MODE_RESPONSE_BYTES;
    memory.response_length = 6;
  } else {
    // For standard polling, simply write the controller data to the console one byte at a time
    memory.response_data = state->input.button_data;

    if (state->analog_mode == CMDigital) {
      memory.response_length = 2;
    } else if (state->analog_mode == CMAnalog) {
      memory.response_length = 6;
    } else if (state->analog_mode == CMAnalogFull) {
      memory.response_length = 18;
    } else {
      memory.response_length = 0;
    }
  }

  return CRInitialized;
}

command_result mp_process(volatile command_packet *packet, controller_state *state) {
  packet->write(memory.response_data[packet->data_index]);
  
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
  if (packet->data_index + 1 != memory.response_length) {
    return CRProcessing;
  }

  return CRCompleted;
}

command_processor command_controller_main_polling = { 
  .id = 0x42,
  .initialize = &mp_initialize,
  .process = &mp_process,
};

// Since the polling behavior of the 43h command is nearly identical to the 42h command (main polling), 
// its processing is implemented as part of the main polling command. Whether to actuate the rumble 
// motors (as in 42h) or enter/exit config mode (as in 43h) is differented based on the command ID.
command_processor command_controller_enter_exit_config = {
  .id = 0x43,
  .initialize = &mp_initialize,
  .process = &mp_process,
};