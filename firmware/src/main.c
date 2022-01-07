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


volatile uint8_t tx_buf[64] = {};
volatile size_t tx_buf_ptr = 0;

volatile uint8_t rx_buf[64] = {};
volatile size_t rx_buf_ptr = 0;

void intercept_write(uint8_t value) {
  // rx_buf[rx_buf_ptr++] = value;
  platform_spi_playstation_write(value);
}

void interrupt_handler() {
  uint8_t command_byte = platform_spi_playstation_read();
  
  // tx_buf[tx_buf_ptr++] = command_byte;
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

  while (true) {
    if (!platform_spi_playstation_selected()) {
      if (packet.id) {
        printf("Most recent data: command=%02X mode=%02X index=%02X\n", packet.id, controller_state_get_mode(&state), packet.command_index);
        // printf("TX [%d]: ", tx_buf_ptr);
        // helper_print_hex_array(tx_buf, tx_buf_ptr);
        // printf("\n");
        // printf("RX [%d]: ", rx_buf_ptr);
        // helper_print_hex_array(rx_buf, rx_buf_ptr);
        // printf("\n");
      }

      // tx_buf_ptr = 0;
      // rx_buf_ptr = 0;

      // Reset the packet
      packet.id = 0x0;
      packet.command_byte = 0x0;
      packet.command_index = -3;
      packet.data_index = -2;
      packet.write = &intercept_write;
      // packet.write = &platform_spi_playstation_write;

      intercept_write(0xFF);
    }
  }

  return 0;
}

#endif