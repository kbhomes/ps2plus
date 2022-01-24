#ifdef PLATFORM_PIC16F

#include "pic16f_platform.h"

uint64_t current_micros;

void pic16f_timing_interrupt_callback() {
  // Timer perios is 10us
  current_micros += 10;
}

void __interrupt(irq(IRQ_TMR0),base(8),high_priority) pic16f_interrupt_timer() {
  TMR0IF = 0;
}

void pic16f_setup_timing() {
  T0EN = 1;
  
  // Set 8-bit mode
  T016BIT = 0;
  
  // Set 1:32 pre-scaling (minimum 2us period)
  T0CKPS3 = 1;
  T0CKPS2 = 0;
  T0CKPS1 = 1;
  T0CKPS0 = 0;
  
  // Set clock source as F_osc / 4
  T0CS2 = 0;
  T0CS1 = 1;
  T0CS0 = 0;
  
  // Set timer period to 10us (2us * (0x04 + 1))
  TMR0H = 0x04;
  TMR0L = 0x00;
  
  // Enable timer 0 interrupt
  TMR0IE = 1;
}

unsigned long platform_timing_millis(void) {
  return current_micros / 1000;
}

void platform_timing_sleep(unsigned int millis) {
  while (millis-- > 0) {
    __delay_ms(1);
  }
}

#endif /* PLATFORM_PIC16F */