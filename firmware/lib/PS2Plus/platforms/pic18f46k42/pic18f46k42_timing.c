#ifdef PLATFORM_PIC18F46K42

#include "pic18f46k42_platform.h"

volatile unsigned long current_millis;

void __interrupt(irq(IRQ_TMR0), base(PIC_IVT_BASE)) pic18f46k42_interrupt_timer() {
  TMR0IF = 0;
  current_millis++;
}

void pic18f46k42_setup_timing() {
  // Set 8-bit mode
  T016BIT = 0;
  
  // Set 1:64 pre-scaling (min. 4us, max. 1.024ms period)
  T0CKPS3 = 0;
  T0CKPS2 = 1;
  T0CKPS1 = 1;
  T0CKPS0 = 0;
  
  // Set clock source as F_osc / 4
  T0CS2 = 0;
  T0CS1 = 1;
  T0CS0 = 0;
  
  // Set timer period to ~1ms (4us * (0xF9 + 1))
  TMR0H = 0xF9;
  TMR0L = 0x00;
  
  // Enable timer 0 interrupt
  TMR0IE = 1;
  
  // Start timer 0
  T0EN = 1;
}

unsigned long platform_timing_millis(void) {
  return current_millis;
}

void platform_timing_sleep(unsigned int millis) {
  while (millis-- > 0) {
    __delay_ms(1);
  }
}

#endif /* PLATFORM_PIC18F46K42 */