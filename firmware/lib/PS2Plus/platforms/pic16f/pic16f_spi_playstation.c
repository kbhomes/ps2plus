#ifdef PLATFORM_PIC16F

#include "pic16f_platform.h"

const pic_digital_io_pin PIN_SPI_SCK = PIC_DIGITAL_IO_PIN(C, 3);
const pic_digital_io_pin PIN_SPI_MOSI = PIC_DIGITAL_IO_PIN(C, 4);
const pic_digital_io_pin PIN_SPI_MISO = PIC_DIGITAL_IO_PIN(C, 5);
const pic_digital_io_pin PIN_SPI_ACK = PIC_DIGITAL_IO_PIN(A, 4);
const pic_digital_io_pin PIN_SPI_ATT = PIC_DIGITAL_IO_PIN(A, 5);

const pic_digital_io_pin PIN_LED = PIC_DIGITAL_IO_PIN(D, 2);

/**
 * @brief SPI interrupt handler, executed when a transmission from the console 
 *        is received and a byte is available to read
 */
void __interrupt(irq(IRQ_SPI1RX)) pic16f_interrupt_spi() {
  pic_digital_io_pin_write(&PIN_LED, PIC_DIGITAL_HIGH);
  
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
  SPI1CON0bits.BMODE = 0;
  SPI1CON0bits.LSBF = 1; // Read/write each byte with LSB-first
  SPI1CON0bits.MST = 0; // Enable SPI in peripheral mode
  
  SPI1CON1bits.SMP = 0; // Input data is sampled in the middle of the clock
  SPI1CON1bits.SSP = 1; // SPI SS is active-low
  SPI1CON1bits.CKE = 1; // Output Data is sampled on the rising edge of the clock
  SPI1CON1bits.CKP = 1; // Clock is HIGH when idle
  
  SPI1CON2bits.SSET = 0;
  SPI1CON2bits.RXR = 1;
  SPI1CON2bits.TXR = 1;
  
  // Enable the correct pin directions
  pic_digital_io_pin_mode(&PIN_SPI_SCK, PICPinMode_Input);
  pic_digital_io_pin_mode(&PIN_SPI_MOSI, PICPinMode_Input);
  pic_digital_io_pin_mode(&PIN_SPI_MISO, PICPinMode_Output);
  pic_digital_io_pin_mode(&PIN_SPI_ATT, PICPinMode_InputPullup);
  pic_digital_io_pin_mode(&PIN_SPI_ACK, PICPinMode_Output);
  
  pic_digital_io_pin_mode(&PIN_LED, PICPinMode_Output);
  pic_digital_io_pin_write(&PIN_LED, PIC_DIGITAL_LOW);
  
  // Set active-low outputs 
  pic_digital_io_pin_write(&PIN_SPI_ACK, PIC_DIGITAL_HIGH);
  
  pps_unlock();
  SPI1SCKPPS = 0x13;  //RC3->SPI1:SCK1;    
  SPI1SSPPS = 0x05;   //RA5->SPI1:SS1;    
  SPI1SDIPPS = 0x14;  //RC4->SPI1:SDI1;
  RC5PPS = 0x1F;      // SPI1:SDO1->RC5;
  pps_lock();
  
  PIR2bits.SPI1RXIF = 0; //Clear SPI1 receive interrupt flag
  IPR2bits.SPI1RXIP = 1; //Set SPI1 receive interrupt to high priority
  PIE2bits.SPI1RXIE = 1; //Enable SPI1 receive interrupt flag
    
  SPI1CON0bits.EN = 1; // Enable SPI
  
  pic_digital_io_pin_write(&PIN_LED, PIC_DIGITAL_LOW);
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
  SPI1TXB = value;
}

#endif /* PLATFORM_PIC16F */