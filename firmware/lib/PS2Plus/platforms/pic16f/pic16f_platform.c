#ifdef PLATFORM_PIC16F

#pragma config FEXTOSC = OFF    // External Oscillator Selection->Oscillator not enabled
#pragma config RSTOSC = HFINTOSC_64MHZ    // Reset Oscillator Selection->HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1
#pragma config MCLRE = EXTMCLR    // MCLR Enable bit->If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR 
#pragma config WDTE = OFF    // WDT operating mode->WDT Disabled; SWDTEN is ignored
#pragma config LVP = OFF    // Low Voltage Programming Enable bit->HV on MCLR/VPP must be used for programming

#include "pic16f_platform.h"

// Interrupt callback to the main program loop
platform_interrupt_callback main_loop_callback = NULL;

void INTERRUPT_Initialize(void) {

}

void platform_init(platform_interrupt_callback callback) {
  main_loop_callback = callback;
  
  // Enable vectored interrupts
  INTCON0bits.GIEH = 1;
  INTCON0bits.GIEL = 1;
  INTCON0bits.IPEN = 1;
  IPR2bits.SPI1RXIP = 1;
  
  // Setup platform internals
  pic16f_setup_spi_playstation();
  pic16f_setup_timing();
  pic16f_setup_uart_serial();
  pic16f_setup_wired_controller();
}

void pic_digital_io_pin_mode(const pic_digital_io_pin *pin, PICPinMode mode) {
  switch (mode) {
    case PICPinMode_Input:
      *(pin->direction) |= 1 << pin->bit;
      *(pin->pullup) &= ~(1 << pin->bit);
      break;
    
    case PICPinMode_InputPullup:
      *(pin->direction) |= 1 << pin->bit;
      *(pin->pullup) |= 1 << pin->bit;
      break;
    
    case PICPinMode_Output:
      *(pin->direction) &= ~(1 << pin->bit);
      *(pin->pullup) &= ~(1 << pin->bit);
      break;
  }
  
  *(pin->analog) &= ~(1 << pin->bit);
}

inline bool pic_digital_io_pin_read(const pic_digital_io_pin *pin) {
  return (*(pin->read) & (PIC_DIGITAL_HIGH << pin->bit)) != PIC_DIGITAL_LOW;
}

inline void pic_digital_io_pin_write(const pic_digital_io_pin *pin, bool value) {
  if (value) {
    *(pin->write) |= (PIC_DIGITAL_HIGH << pin->bit);
  } else {
    *(pin->write) &= ~(PIC_DIGITAL_HIGH << pin->bit);
  }
}

inline void pic_digital_io_pin_pps_input(const pic_digital_io_pin *pin, volatile uint8_t *pps_input) {
  *pps_input = pin->pps_input_channel;
}

inline void pic_digital_io_pin_pps_output(const pic_digital_io_pin *pin, uint8_t pps_output_channel) {
  *(pin->pps_output) = pps_output_channel;
}

#endif /* PLATFORM_PIC16F */