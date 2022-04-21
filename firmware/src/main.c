#include "main.h"

volatile controller_state state;
volatile command_packet packet;

void write_with_ack(uint8_t value) {
  platform_spi_playstation_ack();
  platform_spi_playstation_write(value);
}

void interrupt_handler() {
#if defined(PS2PLUS_FIRMWARE)
  command_packet_step(&packet, &state, platform_spi_playstation_read());
#elif defined(PS2PLUS_BOOTLOADER)
  // DEV: Nothing! Trying to determine whether the right interrupt executes in firmware mode.
#endif
}

void update_controller() {
  // Read digital button values
  for (ps2plus_controller_digital_button button = 0; button < NUM_DIGITAL_BUTTONS; button++) {
    bool active_low_button_state = !platform_controller_read_digital_button(button);
    debounced_force(&state.input.digital_buttons[button], active_low_button_state);
    // debounced_update(&state.input.digital_buttons[button], active_low_button_state);
  }

  // Read joystick values
  for (ps2plus_controller_joystick_axis joystick = 0; joystick < NUM_JOYSTICK_AXES; joystick++) {
    state.input.joysticks[joystick] = platform_controller_read_joystick(joystick);
  }
  
  // Recompute controller state
  controller_input_recompute(&state.input);
  controller_state_update_mode(&state);
  
  // Update the analog mode LED
  platform_controller_set_analog_led(state.analog_mode == CMAnalog || state.analog_mode == CMAnalogFull);
}

void handle_transmission() {
  if (!platform_spi_playstation_selected()) {
    // Prepare for the next packet
    platform_spi_playstation_write(0xFF);
    command_packet_initialize(&packet, &write_with_ack);
  }
}

int main(void) {
  platform_init(&interrupt_handler);
  
  // Set the initial internal state of the firmware
  controller_state_initialize(&state);
  controller_state_set_versions(&state, VERSION_FIRMWARE, VERSION_MICROCONTROLLER, VERSION_CONFIGURATION, VERSION_BOOTLOADER);
  command_packet_initialize(&packet, &write_with_ack);
  
  // Execute the initialization for the bootloader/firmware
  main_init(&state);

  while (true) {
    update_controller();
    handle_transmission();
    main_loop(&state);
  }

  return 0;
}