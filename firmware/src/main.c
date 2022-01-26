#include <commands/packet.h>
#include <controller/state.h>
#include <platforms/platform.h>
#include <builtins.h>

volatile controller_state state;
volatile command_packet packet;

void write_with_ack(uint8_t value) {
  platform_spi_playstation_ack();
  platform_spi_playstation_write(value);
}

void interrupt_handler() {
  command_packet_step(&packet, &state, platform_spi_playstation_read());
}

int main(void) {
  platform_init(&interrupt_handler);
  controller_state_initialize(&state);
  command_packet_initialize(&packet, &write_with_ack);
  
  platform_spi_playstation_write(0b00110101);
  
  while (true) {
    // Update the controller state
    for (controller_input_digital_button button = 0; button < NUM_DIGITAL_BUTTONS; button++) {
      bool active_low_button_state = !platform_controller_read_digital_button(button);
      debounced_update(&state.input.digital_buttons[button], active_low_button_state);
    }
    controller_input_recompute(&state.input);
    controller_state_update_mode(&state);

    if (!platform_spi_playstation_selected()) {
      // Prepare for the next packet
      platform_spi_playstation_write(0xFF);
      command_packet_initialize(&packet, &write_with_ack);
    }
  }

  return 0;
}