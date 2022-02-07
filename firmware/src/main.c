#include <commands/packet.h>
#include <controller/state.h>
#include <platforms/platform.h>

#include "versions.h"

volatile controller_state state;
volatile command_packet packet;

void write_with_ack(uint8_t value) {
  platform_spi_playstation_ack();
  platform_spi_playstation_write(value);
}

void interrupt_handler() {
  command_packet_step(&packet, &state, platform_spi_playstation_read());
}

void print_hex_array(uint8_t *data, size_t length) {
  for (int i = 0; i < length; i++) {
    printf("%02X ", data[i]);
  }
  printf("\n");
}

void check_configuration_version() {
  // If the stored configuration version does not match this firmware's version, reset
  // all custom configuration and save it back to storage
  if (state.custom_config.configuration_version != state.versions.configuration) {
    controller_custom_config_initialize(&state.custom_config);
    controller_custom_config_save(&state.custom_config);
  }
}

int main(void) {
  platform_init(&interrupt_handler);

  // Set the initial internal state of the firmware
  controller_state_initialize(&state);
  controller_state_set_versions(&state, VERSION_FIRMWARE, VERSION_MICROCONTROLLER, VERSION_CONFIGURATION);
  
  // Load the configuration from storage and check the configuration version
//  controller_custom_config_load(&state.custom_config);
  // check_configuration_version();

  command_packet_initialize(&packet, &write_with_ack);
  
  while (true) {
//    // Update the controller state
//    for (controller_input_digital_button button = 0; button < NUM_DIGITAL_BUTTONS; button++) {
//      bool active_low_button_state = !platform_controller_read_digital_button(button);
//      debounced_update(&state.input.digital_buttons[button], active_low_button_state);
//    }
//    controller_input_recompute(&state.input);
    controller_state_update_mode(&state);

    if (!platform_spi_playstation_selected()) {
      // Prepare for the next packet
      platform_spi_playstation_write(0xFF);
      command_packet_initialize(&packet, &write_with_ack);
    }
  }

  return 0;
}