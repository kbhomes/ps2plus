#ifdef PLATFORM_PIC16F

#include "pic16f_platform.h"

#define PIC_SPI_PPS_SCK_REGISTER SPI1SCKPPS   // Table 17-1 in PIC18F46K42 data sheet
#define PIC_SPI_PPS_MOSI_REGISTER SPI1SDIPPS  // Table 17-1 in PIC18F46K42 data sheet
#define PIC_SPI_PPS_ATT_REGISTER SPI1SSPPS    // Table 17-1 in PIC18F46K42 data sheet
#define PIC_SPI_PPS_MISO_CHANNEL 0b011111     // Table 17-2 in PIC18F46K42 data sheet

const pic_digital_io_pin PIN_SPI_SCK = PIC_DIGITAL_IO_PIN(C, 3);
const pic_digital_io_pin PIN_SPI_MOSI = PIC_DIGITAL_IO_PIN(C, 4);
const pic_digital_io_pin PIN_SPI_MISO = PIC_DIGITAL_IO_PIN(C, 5);
const pic_digital_io_pin PIN_SPI_ACK = PIC_DIGITAL_IO_PIN(A, 4);
const pic_digital_io_pin PIN_SPI_ATT = PIC_DIGITAL_IO_PIN(A, 5);

/**
 * @brief SPI interrupt handler, executed when a transmission from the console 
 *        is received and a byte is available to read
 */
void __interrupt(irq(IRQ_SPI1RX)) pic16f_interrupt_spi() {
  // Simply call the main program's data handler if we received an SPI transmission
  main_loop_callback();
}

void pps_unlock() {
  PPSLOCK = 0x55;
  PPSLOCK = 0xAA;
  PPSLOCK = 0; //PPS unlocked	
}

void pps_lock() {
  PPSLOCK = 0x55;
  PPSLOCK = 0xAA;
  PPSLOCK = 1; //PPS locked   	
}

void pic16f_setup_spi_playstation() {
  SPI1TWIDTH = 8;
  SPI1CON0bits.BMODE = 1; // Read 8 bits at a time
  SPI1CON0bits.LSBF = 1; // Read/write each byte with LSB-first
  SPI1CON0bits.MST = 0; // Enable SPI in peripheral mode
  SPI1CON1bits.SMP = 0; // Input data is sampled in the middle of the clock
  SPI1CON1bits.SSP = 1; // SPI SS is active-low
  SPI1CON1bits.CKE = 0; // Output Data is sampled on the rising edge of the clock
  SPI1CON1bits.CKP = 1; // Clock is HIGH when idle
  SPI1CON2bits.RXR = 1; // Enable SPI receive
  SPI1CON2bits.TXR = 1; // Enable SPI transmit
  
  // Enable the correct pin directions
  pic_digital_io_pin_mode(&PIN_SPI_SCK, PICPinMode_Input);
  pic_digital_io_pin_mode(&PIN_SPI_MOSI, PICPinMode_Input);
  pic_digital_io_pin_mode(&PIN_SPI_MISO, PICPinMode_Output);
  pic_digital_io_pin_mode(&PIN_SPI_ATT, PICPinMode_InputPullup);
  pic_digital_io_pin_mode(&PIN_SPI_ACK, PICPinMode_Output);
  
  // Set active-low outputs 
  pic_digital_io_pin_write(&PIN_SPI_ACK, PIC_DIGITAL_HIGH);
  
  // Configure PPS (Peripheral Pin Select) to remap the SPI input and output pins
  pps_unlock();
  pic_digital_io_pin_pps_input(&PIN_SPI_SCK, &PIC_SPI_PPS_SCK_REGISTER);
  pic_digital_io_pin_pps_input(&PIN_SPI_MOSI, &PIC_SPI_PPS_MOSI_REGISTER);
  pic_digital_io_pin_pps_input(&PIN_SPI_ATT, &PIC_SPI_PPS_ATT_REGISTER);
  pic_digital_io_pin_pps_output(&PIN_SPI_MISO, PIC_SPI_PPS_MISO_CHANNEL);
  pps_lock();
  
  IPR2bits.SPI1RXIP = 1; //Set SPI1 receive interrupt to high priority
  PIE2bits.SPI1RXIE = 1; //Enable SPI1 receive interrupt flag
  SPI1STATUSbits.CLRBF = 1; // Clear TX/RX FIFOs
    
  SPI1CON0bits.EN = 1; // Enable SPI
}

void platform_spi_playstation_ack() {
  LATAbits.LA4 = 0;
  __delay_us(2);
  LATAbits.LA4 = 1;
  
//  pic_digital_io_pin_write(&PIN_SPI_ACK, PIC_DIGITAL_LOW);
//  __delay_us(2);
//  pic_digital_io_pin_write(&PIN_SPI_ACK, PIC_DIGITAL_HIGH);
}

bool platform_spi_playstation_selected() {
//  return PORTAbits.RA5 == PIC_DIGITAL_LOW;
  return pic_digital_io_pin_read(&PIN_SPI_ATT) == PIC_DIGITAL_LOW;
}

bool platform_spi_playstation_data_available() {
  return SPI1RXBF;
}

uint8_t platform_spi_playstation_read() {
  return SPI1RXB;
}

void platform_spi_playstation_write(uint8_t value) {
  SPI1STATUSbits.CLRBF = 1;
  SPI1TXB = value;
}

#endif /* PLATFORM_PIC16F */