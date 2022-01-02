#ifdef PLATFORM_ARDUINO

#include "arduino_platform.h"

#define PIN_ACK 4
#define PORT_ACK PORTD
#define PORT_ACK_PIN PORTD4

// Suppressing "unused" warnings for register clear variables
#define UNUSED(X) ((void)(X))

#define DIGITAL_FAST_HIGH(port, pin) port |= (1 << pin)
#define DIGITAL_FAST_LOW(port, pin) port &= ~(1 << pin)
#define PINMODE_FAST_OUTPUT(ddr, pin) ddr |= (1 << pin)
#define PINMODE_FAST_INPUT(ddr, pin) ddr &= ~(1 << pin)

/**
 * Arduino boards use AVR microcontrollers with standard SPI capabilities. High quality
 * and easy-to-understand documentation on these SPI capabilities can be found here:
 * http://avrbeginners.net/architecture/spi/spi.html
 * 
 * Small glossary:
 * 
 * - SPCR: S̲P̲I C̲ontrol R̲egister
 *   - SPE: S̲P̲I E̲nable bit
 *   - SPIE: S̲P̲I I̲nterrupt E̲nable bit
 *     - Must be set to generate hardware interrupts when data is received
 *   - DORD: D̲ata O̲R̲D̲er bit
 *     - Transmits data with bits being ordered LSB-first (1) or MSB-first (0)
 * - SPSR: S̲P̲I S̲tatus R̲egister
 *   - SPIF: S̲P̲I I̲nterrupt F̲lag
 *     - Read-only flag indicating that data was received and an interrupt generated
 *     - Automatically cleared by letting the interrupt handler execute, or by reading SPSR followed by SPDR
 * - SPDR: S̲P̲I D̲ata R̲egister
 *   - Register to transmit or receive data
 */

/**
 * @brief SPI interrupt handler, executed when a transmission from the console 
 *        is received and a byte is available to read
 */
ISR(SPI_STC_vect) {
  // Call the main program's data handler
  interrupt_callback();
}

void arduino_setup_spi_playstation() {
  // Enables SPI with hardware interrupts
  SPCR = (
    _BV(SPE)  | // Enable SPI 
    _BV(SPIE) | // Enable SPI interrupts
    _BV(DORD) | // Read/write each byte with LSB-first
    _BV(CPOL) | // Clock is HIGH when idle
    _BV(CPHA)   // Data is sampled on the rising edge of the clock
  );

  {
    // Clear SPI registers -- primarily, the SPIF flag (indicating that an SPI transfer was completed)
    // is cleared by reading the SPIF bit in the SPSR register, followed by reading the SPDR register.
    char clear;
    clear = SPSR;
    clear = SPDR;
    UNUSED(clear);
  }

  // Enable the correct pin directions
  pinMode(MOSI, INPUT_PULLUP);
  pinMode(MISO, OUTPUT);
  pinMode(SCK, INPUT_PULLUP);
  pinMode(SS, INPUT_PULLUP);
  pinMode(PIN_ACK, OUTPUT);
}

void platform_spi_playstation_ack() {
  DIGITAL_FAST_LOW(PORT_ACK, PORT_ACK_PIN);
  delayMicroseconds(2);
  DIGITAL_FAST_HIGH(PORT_ACK, PORT_ACK_PIN);
}

bool platform_spi_playstation_selected() {
  return digitalRead(SS) == LOW;
}

bool platform_spi_playstation_data_available() {
  return SPSR & _BV(SPIF);
}

uint8_t platform_spi_playstation_read() {
  return SPDR;
}

void platform_spi_playstation_write(uint8_t value) {
  SPDR = value;
}

uint8_t platform_spi_playstation_transmit(uint8_t value) {
  // Write the value out
  SPDR = value;

  // Wait until the transmission has completed
  while (!(SPSR & _BV(SPIF))) {
    delayMicroseconds(1);
  }
  
  // Read the value from the console
  return SPDR;
}

#endif /* PLATFORM_ARDUINO */