#include <commands/command.h>
#include <platforms/platform.h>
#include <controller_state.h>

#ifdef PLATFORM_ARDUINO
#include <Arduino.h>
#define PIN_BUTTON 2
#endif

controller_state state = { 
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

command_packet packet;

command_processor *processor;
bool spi_previously_selected = false;

void interrupt_handler() {
  if (platform_spi_data_available()) {
    uint8_t command_byte = platform_spi_read();

    if (packet.command_index == -3) {
      // Assuming that we will always be sent a 01h from the console
      platform_spi_write(controller_state_get_mode(&state));
    } else if (packet.command_index == -2) {
      // Second header byte is the command ID
      packet.id = command_byte;
      processor = command_find_processor(command_byte);
      
      if (processor->initialize(&state) != CRInitialized) {
        printf("Failed to initialize processor for command %02X\n", command_byte);
        processor = NULL;
      }
      
      platform_spi_write(0x5A);
    } else {
      if (processor) {
        // Let the current command processor take over from here
        command_result result = processor->process(&packet, &state);

        if (result == CRCompleted) {
          printf("Completed processing of command %02X\n", command_byte);
          processor = NULL;
        }
      }
    }

    packet.command_index++;
    packet.data_index++;
  }
}

int main(void) {
  platform_init(&interrupt_handler);

#ifdef PLATFORM_ARDUINO
  while (true) {
    bool spi_currently_selected = digitalRead(SS) == LOW;
    bool spi_newly_selected = spi_previously_selected != spi_currently_selected;

    if (spi_newly_selected) {
      if (spi_currently_selected) {
        // First data byte in the packet is always FFh
        platform_spi_write(0xFF);

        // Reset the command packet structure
        packet.id = 0;
        packet.command_byte = 0x00;
        packet.command_index = -3;
        packet.data_index = -2;
        packet.write = &platform_spi_write;
      }
    }

    spi_previously_selected = spi_currently_selected;
  }
#endif

  return 0;
}