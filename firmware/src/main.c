#ifdef PLATFORM_ARDUINO

#include <commands/command.h>
#include <platforms/platform.h>
#include <controller_state.h>

#include <Arduino.h>

void interrupt_handler() {
  uint8_t read = platform_spi_playstation_transmit(0xFF);
  platform_spi_playstation_ack();

  if (read == 0x01) {
    read = platform_spi_playstation_transmit(0x41);
    platform_spi_playstation_ack();

    if (read == 0x42) {
      read = platform_spi_playstation_transmit(0x5A);
      platform_spi_playstation_ack();

      if (read == 0x00) {
        read = platform_spi_playstation_transmit(0xFF);
        platform_spi_playstation_ack();

        if (read == 0x00) {
          read = platform_spi_playstation_transmit(0xFF);

          if (read == 0x00) {
            read = platform_spi_playstation_transmit(0xFF);
            platform_spi_playstation_ack();

            if (read == 0x01) {
              read = platform_spi_playstation_transmit(0x41);
              platform_spi_playstation_ack();

              printf("In here: %02X\n", read);
            }
          }
        }
      }
    }
  }
}

int main(void) {
  platform_init(&interrupt_handler);

  while (true) {
    delayMicroseconds(8);
  }

  return 0;
}

#endif