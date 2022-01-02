#ifdef PLATFORM_ARDUINO

#include <Arduino.h>
#include <commands/command.h>
#include <controller_state.h>
#include <platforms/platform.h>

volatile int byte_number = 0;

void interrupt_handler() {
  uint8_t read = platform_spi_playstation_read();
  platform_spi_playstation_ack();

  if (byte_number == 0 && read == 0x01) {
    platform_spi_playstation_write(0x41);
  } else if (byte_number == 1 && read == 0x42) {
    platform_spi_playstation_write(0x5A);
  } else if (byte_number == 2 && read == 0x00) {
    platform_spi_playstation_write(0xFF);
  } else if (byte_number == 3 && read == 0x00) {
    platform_spi_playstation_write(0xFF);
  } else if (byte_number == 4 && read == 0x00) {
    platform_spi_playstation_write(0xFF);
    printf("In here: %02X\n", read);
  }

  byte_number++;
}

int main(void) {
  platform_init(&interrupt_handler);
  byte_number = 0;

  while (true) {
    if (!platform_spi_playstation_selected()) {
      byte_number = 0;
      platform_spi_playstation_write(0xFF);
    }

    delayMicroseconds(1);
  }

  return 0;
}

#endif