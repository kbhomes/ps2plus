#ifdef PLATFORM_ARDUINO

#include <platforms/platform.h>
#include <Arduino.h>
#include <avr/eeprom.h>

// Use the AVR baud rate utility header
#define F_CPU 16000000UL
#define BAUD 9600
#include <util/setbaud.h>

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

platform_interrupt_callback interrupt_callback;

ISR(SPI_STC_vect) {
  interrupt_callback();
}

void setup_spi_peripheral_mode() {
  // Enables SPI with hardware interrupts
  SPCR = _BV(SPE) | _BV(DORD) | _BV(CPOL) | _BV(CPHA);

  // Clear SPI Registers
  {
    volatile char clr;
    clr = SPSR;
    clr = SPDR;
  }

  // Enable the correct pin directions
  pinMode(MOSI, INPUT_PULLUP);
  pinMode(MISO, OUTPUT);
  pinMode(SCK, INPUT_PULLUP);
  pinMode(SS, INPUT_PULLUP);

  SPCR |= _BV(SPIE);
}

void uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

char uart_getchar(FILE *stream) {
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
}

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

void setup_uart_serial_communication() {
  // https://appelsiini.net/2011/simple-usart-with-avr-libc/

  // Enable UART in the hardware
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;

#if USE_2X
  UCSR0A |= _BV(U2X0);
#else
  UCSR0A &= ~(_BV(U2X0));
#endif

  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */

  // Redirect stdin and stdout through the UART communication headers
  stdout = &uart_output;
  stdin = &uart_input;
}

void platform_init(platform_interrupt_callback callback) {
  init();

#ifdef UNIT_TEST
  // Unit tests on an Arduino board require a delay during startup
  delay(2000);
#endif

  interrupt_callback = callback;
  setup_spi_peripheral_mode();
  setup_uart_serial_communication();
}

unsigned long platform_timing_millis(void) {
  return millis();
}

void platform_timing_sleep(unsigned int millis) {
  delay(millis);
}

bool platform_spi_selected() {
  return digitalRead(SS) == LOW;
}

bool platform_spi_data_available() {
  return SPSR & _BV(SPIF);
}

uint8_t platform_spi_read() {
  return SPDR;
}

void platform_spi_write(uint8_t value) {
  SPDR = value;
}

uint8_t platform_memory_read(size_t address) {
  return eeprom_read_byte((uint8_t *)address);
}

void platform_memory_write(size_t address, uint8_t byte) {
  eeprom_update_byte((uint8_t *)address, byte);
}

#endif