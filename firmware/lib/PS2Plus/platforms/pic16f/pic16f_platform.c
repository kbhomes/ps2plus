#ifdef PLATFORM_PIC16F

#include "pic16f_platform.h"

// Using the XC8 compiler like so worked?
// xc8-cc -mcpu=16F877A -Ilib/PS2Plus -DPLATFORM_PIC16F ./src/main.c ./lib/PS2Plus/commands/button_inclusions_command.c ./lib/PS2Plus/commands/command.c ./lib/PS2Plus/commands/configur e_analog_response_command.c ./lib/PS2Plus/commands/device_descriptor_46_command.c ./lib/PS2Plus/commands/device_descriptor_47_command.c ./lib/PS2Plus/commands/device_descriptor_4c_com mand.c ./lib/PS2Plus/commands/initialize_pressure_sensor_command.c ./lib/PS2Plus/commands/main_polling_command.c ./lib/PS2Plus/commands/map_motors_command.c ./lib/PS2Plus/commands/sta tus_info_command.c ./lib/PS2Plus/commands/switch_digital_analog_mode_command.c ./lib/PS2Plus/controller_input.c ./lib/PS2Plus/controller_state.c ./lib/PS2Plus/platforms/arduino.c ./li b/PS2Plus/platforms/native.c ./lib/PS2Plus/platforms/pic16f.c ./lib/PS2Plus/utilities/debounced_bool.c ./lib/PS2Plus/utilities/latching_bool.c

// Interrupt callback to the main program loop
platform_interrupt_callback main_loop_callback = NULL;

void platform_init(platform_interrupt_callback callback) {
  main_loop_callback = callback;
  
  // Setup platform internals
  pic16f_setup_spi_playstation();
  pic16f_setup_timing();
  pic16f_setup_uart_serial();
  pic16f_setup_wired_controller();
  
  // Enable all interrupts
  IVTLOCK = 0x55;
  IVTLOCK = 0xAA;
  IVTLOCKbits.IVTLOCKED = 0x00; // unlock IVT

  IVTBASEU = 0;
  IVTBASEH = 0;
  IVTBASEL = 8;

  IVTLOCK = 0x55;
  IVTLOCK = 0xAA;
  IVTLOCKbits.IVTLOCKED = 0x01; // lock IVT

  GIE = 1;
  
  // Assign peripheral interrupt priority vectors
  IPR3bits.TMR0IP = 1;
  IPR2bits.SPI1IP = 1;
  IPR2bits.SPI1RXIP = 1;
  
  // NOSC HFINTOSC; NDIV 1; 
  OSCCON1 = 0x60;
  // CSWHOLD may proceed; SOSCPWR Low power; 
  OSCCON3 = 0x00;
  // MFOEN disabled; LFOEN disabled; ADOEN disabled; SOSCEN disabled; EXTOEN disabled; HFOEN disabled; 
  OSCEN = 0x00;
  // HFFRQ 64_MHz; 
  OSCFRQ = 0x08;
  // TUN 0; 
  OSCTUNE = 0x00;
  
  // CLKRMD CLKR enabled; SYSCMD SYSCLK enabled; SCANMD SCANNER enabled; FVRMD FVR enabled; IOCMD IOC enabled; CRCMD CRC enabled; HLVDMD HLVD enabled; NVMMD NVM enabled; 
  PMD0 = 0x00;
  // NCO1MD DDS(NCO1) enabled; TMR0MD TMR0 enabled; TMR1MD TMR1 enabled; TMR4MD TMR4 enabled; TMR5MD TMR5 enabled; TMR2MD TMR2 enabled; TMR3MD TMR3 enabled; TMR6MD TMR6 enabled; 
  PMD1 = 0x00;
  // ZCDMD ZCD enabled; DACMD DAC enabled; CMP1MD CMP1 enabled; ADCMD ADC enabled; CMP2MD CMP2 enabled; 
  PMD2 = 0x00;
  // CCP2MD CCP2 enabled; CCP1MD CCP1 enabled; CCP4MD CCP4 enabled; CCP3MD CCP3 enabled; PWM6MD PWM6 enabled; PWM5MD PWM5 enabled; PWM8MD PWM8 enabled; PWM7MD PWM7 enabled; 
  PMD3 = 0x00;
  // CWG3MD CWG3 enabled; CWG2MD CWG2 enabled; CWG1MD CWG1 enabled; 
  PMD4 = 0x00;
  // U2MD UART2 enabled; U1MD UART1 enabled; SPI1MD SPI1 enabled; I2C2MD I2C2 enabled; I2C1MD I2C1 enabled; 
  PMD5 = 0x00;
  // DSMMD DSM1 enabled; CLC3MD CLC3 enabled; CLC4MD CLC4 enabled; SMT1MD SMT1 enabled; CLC1MD CLC1 enabled; CLC2MD CLC2 enabled; 
  PMD6 = 0x00;
  // DMA1MD DMA1 enabled; DMA2MD DMA2 enabled; 
  PMD7 = 0x00;
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
}

bool pic_digital_io_pin_read(const pic_digital_io_pin *pin) {
  return (*(pin->read) & (PIC_DIGITAL_HIGH << pin->bit)) != PIC_DIGITAL_LOW;
}

void pic_digital_io_pin_write(const pic_digital_io_pin *pin, bool value) {
  if (value) {
    *(pin->write) |= (PIC_DIGITAL_HIGH << pin->bit);
  } else {
    *(pin->write) &= ~(PIC_DIGITAL_HIGH << pin->bit);
  }
}

#endif /* PLATFORM_PIC16F */