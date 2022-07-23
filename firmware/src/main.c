#ifndef PS2PLUS_TEST

#include "main.h"

#include <math.h>

volatile controller_state state;
volatile command_packet packet;

void interrupt_handler() {
  command_packet_step(&packet, &state, platform_spi_playstation_read());
  state.last_communication_time = platform_timing_millis();
}

void handle_transmission() {
  if (!platform_spi_playstation_selected()) {
    // Prepare for the next packet
    platform_spi_playstation_write(0xFF);
    command_packet_initialize(&packet, &platform_spi_playstation_ack_write);
  }
}

int main(void) {
  platform_init(&interrupt_handler);
  
  // Set the initial internal state
  controller_state_initialize(&state);
  controller_state_set_versions(&state, VERSION_FIRMWARE, VERSION_MICROCONTROLLER, VERSION_CONFIGURATION, VERSION_BOOTLOADER);
  command_packet_initialize(&packet, &platform_spi_playstation_ack_write);
  
  // Execute the initialization for the bootloader/firmware
  main_init(&state);

  while (true) {
    handle_transmission();
    main_loop(&state);
  }

  return 0;
}

#endif