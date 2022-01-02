#ifdef PLATFORM_ARDUINO

#include <commands/command.h>
#include <platforms/platform.h>
#include <controller_state.h>

#include <Arduino.h>
#define PIN_ACK 4

#define DIGITAL_FAST_HIGH(port, pin) port |= (1 << pin)
#define DIGITAL_FAST_LOW(port, pin) port &= ~(1 << pin)
#define PINMODE_FAST_OUTPUT(ddr, pin) ddr |= (1 << pin)
#define PINMODE_FAST_INPUT(ddr, pin) ddr &= ~(1 << pin)

void ack_input() { PINMODE_FAST_INPUT(DDRD, PORTD4); }
void ack_output() { PINMODE_FAST_OUTPUT(DDRD, PORTD4); }
void ack_high() { DIGITAL_FAST_HIGH(PORTD, PORTD4); }
void ack_low() { DIGITAL_FAST_LOW(PORTD, PORTD4); }

void ack_config() {
  // ack_input();
  // ack_low();
  ack_output();
  ack_high();
}

void ack() {
  // ack_output();
  // delayMicroseconds(2);
  // ack_input();
  
  ack_low();
  delayMicroseconds(2);
  ack_high();
}

int main(void) {
  platform_init(NULL);
  ack_config();

  while (true) {
    uint8_t read = platform_spi_transmit(0xFF);
    ack_low();
    ack_high();

    if (read == 0x01) {
      read = platform_spi_transmit(0x41);
      ack_low();
      ack_high();

      if (read == 0x42) {
        read = platform_spi_transmit(0x5A);
        ack_low();
        ack_high();

        if (read == 0x00) {
          read = platform_spi_transmit(0xFF);
          ack_low();
          ack_high();

          if (read == 0x00) {
            read = platform_spi_transmit(0xFF);

            if (read == 0x00) {
              read = platform_spi_transmit(0xFF);
              ack_low();
              ack_high();

              if (read == 0x01) {
                read = platform_spi_transmit(0x41);
                ack_low();
                ack_high();

                printf("In here: %02X\n", read);
              }
            }
          }
        }
      }
    }
  }

  return 0;
}

#endif