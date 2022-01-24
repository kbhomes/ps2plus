#ifdef PLATFORM_PIC16F

#include "pic16f_platform.h"

const pic_digital_io_pin PIN_SPI_SCK = PIC_DIGITAL_IO_PIN(C, 3);
const pic_digital_io_pin PIN_SPI_MOSI = PIC_DIGITAL_IO_PIN(C, 4);
const pic_digital_io_pin PIN_SPI_MISO = PIC_DIGITAL_IO_PIN(C, 5);
const pic_digital_io_pin PIN_SPI_ACK = PIC_DIGITAL_IO_PIN(A, 4);
const pic_digital_io_pin PIN_SPI_ATT = PIC_DIGITAL_IO_PIN(A, 5);

/**
 * @brief SPI interrupt handler, executed when a transmission from the console 
 *        is received and a byte is available to read
 */
void __interrupt(irq(IRQ_SPI1RX),base(8),high_priority) pic16f_interrupt_spi() {
  // Simply call the main program's data handler if we received an SPI transmission
  main_loop_callback();
}

void pic16f_setup_spi_playstation() {
  SPI1CON0 = 0;
  SPI1CON0bits.LSBF = 1; // Read/write each byte with LSB-first
    
  SPI1CON1 = 0;
  SPI1CON1bits.SSP = 1; // Enable SPI in peripheral mode
  SPI1CON1bits.CKE = 1; // Data is sampled on the rising edge of the clock
  SPI1CON1bits.CKP = 1; // Clock is HIGH when idle
  
  // Enable the correct pin directions
  pic_digital_io_pin_mode(&PIN_SPI_SCK, PICPinMode_Input);
  pic_digital_io_pin_mode(&PIN_SPI_MOSI, PICPinMode_Input);
  pic_digital_io_pin_mode(&PIN_SPI_MISO, PICPinMode_Output);
  pic_digital_io_pin_mode(&PIN_SPI_ATT, PICPinMode_InputPullup);
  pic_digital_io_pin_mode(&PIN_SPI_ACK, PICPinMode_Output);
  
  // Set active-low outputs 
  pic_digital_io_pin_write(&PIN_SPI_ACK, PIC_DIGITAL_HIGH);
  
  // Clear SPI registers -- primarily, the SPIF flag (indicating that an SPI transfer was completed)
  SPI1IF = 0;
  SPI1RXIF = 0;
  
  // Enable SPI RX interrupts
  SPI1IE = 1;
  SPI1RXIE = 1;
  
  SPI1SCKPPS = 0x13;   //RC3->SPI1:SCK1;    
  SPI1SSPPS = 0x05;   //RA5->SPI1:SS1;    
  SPI1SDIPPS = 0x14;   //RC4->SPI1:SDI1;
  RC5PPS = 0b011111;       // SPI1:SDO1->RC5;
  
  SPI1CON0bits.EN = 1; // Enable SPI
}

void platform_spi_playstation_ack() {
  pic_digital_io_pin_write(&PIN_SPI_ACK, PIC_DIGITAL_LOW);
  __delay_us(2);
  pic_digital_io_pin_write(&PIN_SPI_ACK, PIC_DIGITAL_HIGH);
}

bool platform_spi_playstation_selected() {
  return pic_digital_io_pin_read(&PIN_SPI_ATT) == PIC_DIGITAL_LOW;
}

bool platform_spi_playstation_data_available() {
  return SPI1RXBF;
}

uint8_t platform_spi_playstation_read() {
  return SPI1RXB;
}

void platform_spi_playstation_write(uint8_t value) {
  while (!platform_spi_playstation_data_available());
  SPI1TXB = value;
}

#endif /* PLATFORM_PIC16F */