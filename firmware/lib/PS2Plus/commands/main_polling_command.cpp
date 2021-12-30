#include "command.h"
#include <stdio.h>

struct {
  uint8_t controller_input_bytes[18];
  size_t controller_input_length;
} mpc_memory;

/**
 * @brief Converts the current controller input into the array of bytes that will
 *        be sent to the console as part of the polling command
 */
void mpc_read_controller_input_bytes(controller_state *state) {
  int index = 0;

  // Store digital button information
  uint16_t digital = controller_input_as_digital(&state->input);
  mpc_memory.controller_input_bytes[index++] = (digital >> 8) & 0xFF;
  mpc_memory.controller_input_bytes[index++] = digital & 0xFF;

  if (controller_state_is_analog(state)) {
    // Store analog joystick information
    for (int j = 0; j < NUM_JOYSTICK_AXES; j++) {
      mpc_memory.controller_input_bytes[index++] = state->input.joysticks[j];
    }

    if (controller_state_include_button_pressure(state)) {
      // Store button pressure information (which will be converted to pure 00h or FFh)
      for (unsigned int b = 0; b < (sizeof(DIGITAL_BUTTON_TO_PRESSURE_INDEX_MAP) / sizeof(controller_input_digital_button)); b++) {
        controller_input_digital_button button = DIGITAL_BUTTON_TO_PRESSURE_INDEX_MAP[b];
        uint8_t digital_value = debounced_read(&state->input.digital_buttons[button]);
        mpc_memory.controller_input_bytes[index++] = (digital_value == 1) ? 0xFF : 0x00;
      }
    }
  }

  // Keep track of how many controller input bytes will be sent to the console
  mpc_memory.controller_input_length = index;
}

command_result mpc_initialize(controller_state *state) {
  // Read the controller input to memory to be sent as the response to this command
  mpc_read_controller_input_bytes(state);

  return CRInitialized;
}

command_result mpc_process(command_packet *packet, controller_state *state) {
  // For standard polling, simply write data to the console one byte at a time
  packet->write(mpc_memory.controller_input_bytes[packet->data_index]);

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
  if (packet->data_index + 1 != mpc_memory.controller_input_length) {
    return CRProcessing;
  }

  return CRCompleted;
}

command_processor main_polling_command = { 
  .id = 0x42,
  .initialize = &mpc_initialize,
  .process = &mpc_process,
};