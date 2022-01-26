#ifndef PIC16F_PLATFORM_H
#define	PIC16F_PLATFORM_H

// CONFIG5H

#include <xc.h>

#include <platforms/platform.h>
#include <conio.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define _XTAL_FREQ 64000000

#define PIC_DIGITAL_IO_PIN(PRT, PIN) \
    { .bit = PIN, .read = &PORT##PRT, .write = &LAT##PRT, .direction = &TRIS##PRT, .pullup = &WPU##PRT, .analog = &ANSEL##PRT }

#define PIC_DIGITAL_HIGH 1
#define PIC_DIGITAL_LOW 0

#define PIC_ANALOG_ENABLE 1
#define PIC_ANALOG_DISABLE 0

// Suppressing "unused" warnings for register clear variables
#define UNUSED(X) ((void)(X))

typedef struct {
  const uint8_t bit;
  volatile uint8_t *enable;
  const uint8_t channel;
} pic_analog_io_pin;

typedef struct {
  const uint8_t bit;
  volatile uint8_t *read;      // PORTx
  volatile uint8_t *write;     // LATx
  volatile uint8_t *direction; // TRISx
  volatile uint8_t *pullup;    // WPUx
  volatile uint8_t *analog;    // ANSELx
} pic_digital_io_pin;

typedef enum {
  PICPinMode_Input,
  PICPinMode_InputPullup,
  PICPinMode_Output,
} PICPinMode;

void pic_digital_io_pin_mode(const pic_digital_io_pin *pin, PICPinMode mode);
inline bool pic_digital_io_pin_read(const pic_digital_io_pin *pin);
inline void pic_digital_io_pin_write(const pic_digital_io_pin *pin, bool value);

/**
 * @brief Interrupt callback provided by the main program, to be called by the platform's SPI
 *        interrupt when data is available to read from the console
 */
extern platform_interrupt_callback main_loop_callback;

void pic16f_setup_spi_playstation();
void pic16f_setup_timing();
void pic16f_setup_uart_serial();
void pic16f_setup_wired_controller();

#endif	/* PIC16F_PLATFORM_H */

