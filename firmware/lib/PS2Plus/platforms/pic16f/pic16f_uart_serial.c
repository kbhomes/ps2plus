#ifdef PLATFORM_PIC16F

#include "pic16f_platform.h"

//void uart_putchar(char c, FILE *stream) {
//    if (c == '\n') {
//        uart_putchar('\r', stream);
//    }
//    
//    // Wait for transmit availability
//    while(!TRMT);
//    
//    TXREG = c;
//}
//
//char uart_getchar(FILE *stream) {
//    // Wait for available data
//    while (!RCIF);
//
//    // Clear overrun error bit
//    if (OERR) {  
//      CREN = 0;
//      CREN = 1;
//    }
//    
//    return RCREG;
//}

//FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
//FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

void pic16f_setup_uart_serial() {
//  // https://appelsiini.net/2011/simple-usart-with-avr-libc/
//
//  // Enable UART in the hardware
//  UBRR0H = UBRRH_VALUE;
//  UBRR0L = UBRRL_VALUE;
//
//#if USE_2X
//  UCSR0A |= _BV(U2X0);
//#else
//  UCSR0A &= ~(_BV(U2X0));
//#endif
//
//  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
//  UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
//
//  // Redirect stdin and stdout through the UART communication headers
//  stdout = &uart_output;
//  stdin = &uart_input;
}

#endif /* PLATFORM_PIC16F */