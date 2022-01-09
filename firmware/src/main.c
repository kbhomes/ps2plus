#ifdef PLATFORM_ARDUINO

#include <Arduino.h>
#include <commands/command.h>
#include <controller_state.h>
#include <platforms/platform.h>

volatile controller_state state = { 
  .input = {
    .digital_buttons = { 0 },
    .joysticks = { 0 },
  },
  .analog_mode = CMDigital,
  .analog_mode_locked = false,
  .config_mode = false,
  .rumble_motor_small = { .mapping = 0xFF, .value = 0x00 },
  .rumble_motor_large = { .mapping = 0xFF, .value = 0x00 },
};
volatile command_packet packet;
command_processor *processor = NULL;

void interrupt_handler() {
  uint8_t command_byte = platform_spi_playstation_read();
  packet.command_byte = command_byte;

  if (packet.command_index == -3) {
    // Assuming that we will always be sent a 01h from the console
    if (command_byte != 0x01) {
      return;
    }

    platform_spi_playstation_ack();
    packet.write(controller_state_get_mode(&state));
  } else if (packet.command_index == -2) {
    platform_spi_playstation_ack();
    packet.write(0x5A);

    // Second header byte is the command ID
    packet.id = command_byte;
    processor = command_find_processor(command_byte);

    if (processor) {
      processor->initialize(&state);
    }
  } else {
    // Let the current command processor take over from here
    if (processor) {
      platform_spi_playstation_ack();
      command_result result = processor->process(&packet, &state);

      if (result == CRCompleted) {
        processor = NULL;
      } 
    }
  }
  
  packet.command_index++;
  packet.data_index++;
}

void helper_print_hex_array(const uint8_t *array, size_t length) {
  for (size_t i = 0; i < length; i++) {
    printf("%02X ", array[i]);
  }
}

int main(void) {
  platform_init(&interrupt_handler);
  controller_input_initialize(&state.input);

  // Reset the packet
  packet.id = 0x0;
  packet.command_byte = 0x0;
  packet.command_index = -3;
  packet.data_index = -2;
  packet.write = &platform_spi_playstation_write;

  pinMode(40, INPUT_PULLUP); // Triangle
  pinMode(38, INPUT_PULLUP); // Circle
  pinMode(36, INPUT_PULLUP); // Cross
  pinMode(34, INPUT_PULLUP); // Square
  pinMode(41, INPUT_PULLUP); // Up
  pinMode(39, INPUT_PULLUP); // Down
  pinMode(37, INPUT_PULLUP); // Left
  pinMode(35, INPUT_PULLUP); // Right
  pinMode(33, INPUT_PULLUP); // Select
  pinMode(31, INPUT_PULLUP); // Start

  while (true) {
    debounced_update(&state.input.digital_buttons[DBTriangle], digitalRead(40));
    debounced_update(&state.input.digital_buttons[DBCircle], digitalRead(38));
    debounced_update(&state.input.digital_buttons[DBCross], digitalRead(36));
    debounced_update(&state.input.digital_buttons[DBSquare], digitalRead(34));
    debounced_update(&state.input.digital_buttons[DDUp], digitalRead(41));
    debounced_update(&state.input.digital_buttons[DDDown], digitalRead(39));
    debounced_update(&state.input.digital_buttons[DDLeft], digitalRead(37));
    debounced_update(&state.input.digital_buttons[DDRight], digitalRead(35));
    debounced_update(&state.input.digital_buttons[DBSelect], digitalRead(33));
    debounced_update(&state.input.digital_buttons[DBStart], digitalRead(31));
    controller_input_recompute(&state.input);

    // helper_print_hex_array(state.input.button_data, sizeof(state.input.button_data));
    // printf("\n");

    if (!platform_spi_playstation_selected()) {
      platform_spi_playstation_write(0xFF);

      // Reset the packet
      packet.id = 0x0;
      packet.command_byte = 0x0;
      packet.command_index = -3;
      packet.data_index = -2;
    }
  }

  return 0;
}

#endif