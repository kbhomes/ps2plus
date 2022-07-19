#ifdef PLATFORM_PIC18F46K42

#pragma config SAFEN = OFF      // Storage Area Flash enable bit (SAF disabled)
#pragma config FEXTOSC = OFF    // External Oscillator Selection->Oscillator not enabled
#pragma config RSTOSC = HFINTOSC_64MHZ    // Reset Oscillator Selection->HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1
#pragma config MCLRE = EXTMCLR    // MCLR Enable bit->If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR 
#pragma config WDTE = OFF    // WDT operating mode->WDT Disabled; SWDTEN is ignored
#pragma config IVT1WAY = OFF // Interrupt vector table lock bit can be repeatedly set and cleared
#pragma config LVP = OFF    // Low Voltage Programming Enable bit->HV on MCLR/VPP must be used for programming

#include "pic18f46k42_platform.h"

bool __debug_is_reset() {
  bool reset_power = STATUSbits.TO && STATUSbits.PD && !PCON0bits.POR;
  bool reset_brownout = STATUSbits.TO && STATUSbits.PD && !PCON0bits.BOR;
  bool reset_mclr_normal = !PCON0bits.RMCLR;
  bool reset_mclr_sleep = STATUSbits.TO && !STATUSbits.PD && !PCON0bits.RMCLR;
  bool reset_wwdt_timeout = !STATUSbits.TO && !PCON0bits.RWDT;
  bool reset_wwdt_violation = !PCON0bits.WDTWV;
  bool reset_instruction = !PCON0bits.RI;
  bool reset_stack_overflow = PCON0bits.STKOVF;
  bool reset_stack_underflow = PCON0bits.STKUNF;
  bool reset_memory_violation = !PCON1bits.MEMV;

  return (
    reset_power ||
    reset_brownout ||
    reset_mclr_normal ||
    reset_mclr_sleep ||
    reset_wwdt_timeout ||
    reset_wwdt_violation ||
    reset_instruction ||
    reset_stack_overflow ||
    reset_stack_underflow ||
    reset_memory_violation
  );
}

// Interrupt callback to the main program loop
platform_interrupt_callback main_loop_callback = NULL;

void platform_init(platform_interrupt_callback callback) {
  main_loop_callback = callback;
  
  // Setup platform internals
  pic18f46k42_setup_spi_playstation();
  pic18f46k42_setup_timing();
  pic18f46k42_setup_uart_serial();
  pic18f46k42_setup_wired_controller();
  
  // Enable vectored interrupts
  INTCON0bits.GIEH = 1;
  INTCON0bits.GIEL = 1;
  INTCON0bits.IPEN = 1;
  IPR2bits.SPI1RXIP = 1;
}

void platform_reset(void) {
  RESET();
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

#endif /* PLATFORM_PIC18F46K42 */