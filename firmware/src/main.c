#include <commands/command.h>
#include <platforms/platform.h>
#include <controller_state.h>

#ifdef PLATFORM_ARDUINO
#include <Arduino.h>
#define PIN_ACK 2
#endif

#define DELAY_8_CYCLES()                                                       \
  \
{                                                                         \
    __asm__("nop\n\t");                                                        \
    __asm__("nop\n\t");                                                        \
    __asm__("nop\n\t");                                                        \
    __asm__("nop\n\t");                                                        \
    __asm__("nop\n\t");                                                        \
    __asm__("nop\n\t");                                                        \
    __asm__("nop\n\t");                                                        \
    __asm__("nop\n\t");                                                        \
  \
}

#define DELAY_1_US()                                                           \
  {                                                                            \
    DELAY_8_CYCLES();                                                          \
    DELAY_8_CYCLES();                                                          \
  }

#define DELAY_2_US()                                                           \
  \
{                                                                         \
    DELAY_1_US();                                                              \
    DELAY_1_US();                                                              \
  \
}

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
volatile command_processor *processor;

void interrupt_handler() {
  if (!platform_spi_data_available()) 
    return;
    
  uint8_t command_byte = platform_spi_read();
  uint8_t packet_id = packet.id;

  digitalWrite(PIN_ACK, LOW);
  digitalWrite(PIN_ACK, HIGH);

  if (packet.command_index == -3) {
    printf("TX 1: %02X\n", command_byte);

    // Assuming that we will always be sent a 01h from the console
    uint8_t mode = controller_state_get_mode(&state);
    platform_spi_write(mode);
    printf("RX 2: %02X\n", mode);
  } else if (packet.command_index == -2) {
    // Second header byte is the command ID
    printf("TX 2: %02X\n", command_byte);
    packet.id = command_byte;
    processor = command_find_processor(command_byte);
    
    if (processor == NULL) {
      printf("Failed to find processor for command %02X\n", command_byte);
    } else if (processor->initialize(&state) != CRInitialized) {
      printf("Failed to initialize processor for command %02X\n", command_byte);
      processor = NULL;
    }
    
    platform_spi_write(0x5A);
    printf("RX 3: %02X\n", 0x5A);
  } else {
    if (processor) {
      printf("TX %d: %02X\n", packet.command_index + 3, command_byte);
      // Let the current command processor take over from here
      packet.command_byte = command_byte;
      command_result result = processor->process(&packet, &state);

      if (result == CRCompleted) {
        printf("Completed processing of command %02X\n", packet_id);
        processor = NULL;

        // Reset the command packet structure
        packet.id = 0;
        packet.command_byte = 0x00;
        packet.command_index = -3;
        packet.data_index = -2;
        packet.write = &platform_spi_write;
        return;
      }
    } else {
      printf("No processor for ID: %02X\n", packet.id);
    }
  }

  packet.command_index++;
  packet.data_index++;
}

int main(void) {
  platform_init(&interrupt_handler);
  bool spi_previously_selected = false;

#ifdef PLATFORM_ARDUINO
  pinMode(PIN_ACK, OUTPUT);
  digitalWrite(PIN_ACK, HIGH);
  controller_input_initialize(&state.input);

  // Reset the command packet structure
  packet.id = 0;
  packet.command_byte = 0x00;
  packet.command_index = -3;
  packet.data_index = -2;
  packet.write = &platform_spi_write;

  while (true) {
    bool spi_currently_selected = platform_spi_selected();
    bool spi_newly_selected = spi_currently_selected && !spi_previously_selected;
    bool spi_newly_deselected = !spi_currently_selected && spi_previously_selected;

    if (spi_newly_selected) {
      // First data byte in the packet is always FFh
      platform_spi_write(0xFF);
    } else if (spi_newly_deselected) {
      packet.id = 0;
      packet.command_byte = 0x00;
      packet.command_index = -3;
      packet.data_index = -2;
      packet.write = &platform_spi_write;
    }

    spi_previously_selected = spi_currently_selected;

    interrupt_handler();
  }
#endif

  return 0;
}