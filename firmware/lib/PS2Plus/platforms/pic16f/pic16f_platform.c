#ifdef PLATFORM_PIC16F

// CONFIG1L
#pragma config FEXTOSC = OFF    // External Oscillator Selection->Oscillator not enabled
#pragma config RSTOSC = HFINTOSC_64MHZ    // Reset Oscillator Selection->HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1

// CONFIG1H
#pragma config CLKOUTEN = OFF    // Clock out Enable bit->CLKOUT function is disabled
#pragma config PR1WAY = ON    // PRLOCKED One-Way Set Enable bit->PRLOCK bit can be cleared and set only once
#pragma config CSWEN = ON    // Clock Switch Enable bit->Writing to NOSC and NDIV is allowed
#pragma config FCMEN = ON    // Fail-Safe Clock Monitor Enable bit->Fail-Safe Clock Monitor enabled

// CONFIG2L
#pragma config MCLRE = EXTMCLR    // MCLR Enable bit->If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR 
#pragma config PWRTS = PWRT_OFF    // Power-up timer selection bits->PWRT is disabled
#pragma config MVECEN = ON    // Multi-vector enable bit->Multi-vector enabled, Vector table used for interrupts
#pragma config IVT1WAY = ON    // IVTLOCK bit One-way set enable bit->IVTLOCK bit can be cleared and set only once
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit->ULPBOR disabled
#pragma config BOREN = SBORDIS    // Brown-out Reset Enable bits->Brown-out Reset enabled , SBOREN bit is ignored

// CONFIG2H
#pragma config BORV = VBOR_2P45    // Brown-out Reset Voltage Selection bits->Brown-out Reset Voltage (VBOR) set to 2.45V
#pragma config ZCD = OFF    // ZCD Disable bit->ZCD disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON
#pragma config PPS1WAY = ON    // PPSLOCK bit One-Way Set Enable bit->PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle
#pragma config STVREN = ON    // Stack Full/Underflow Reset Enable bit->Stack full/underflow will cause Reset
#pragma config DEBUG = OFF    // Debugger Enable bit->Background debugger disabled
#pragma config XINST = OFF    // Extended Instruction Set Enable bit->Extended Instruction Set and Indexed Addressing Mode disabled

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31    // WDT Period selection bits->Divider ratio 1:65536; software control of WDTPS
#pragma config WDTE = OFF    // WDT operating mode->WDT Disabled; SWDTEN is ignored

// CONFIG3H
#pragma config WDTCWS = WDTCWS_7    // WDT Window Select bits->window always open (100%); software control; keyed access not required
#pragma config WDTCCS = SC    // WDT input clock selector->Software Control

// CONFIG4L
#pragma config BBSIZE = BBSIZE_512    // Boot Block Size selection bits->Boot Block size is 512 words
#pragma config BBEN = OFF    // Boot Block enable bit->Boot block disabled
#pragma config SAFEN = OFF    // Storage Area Flash enable bit->SAF disabled
#pragma config WRTAPP = OFF    // Application Block write protection bit->Application Block not write protected

// CONFIG4H
#pragma config WRTB = OFF    // Configuration Register Write Protection bit->Configuration registers (300000-30000Bh) not write-protected
#pragma config WRTC = OFF    // Boot Block Write Protection bit->Boot Block (000000-0007FFh) not write-protected
#pragma config WRTD = OFF    // Data EEPROM Write Protection bit->Data EEPROM not write-protected
#pragma config WRTSAF = OFF    // SAF Write protection bit->SAF not Write Protected
#pragma config LVP = OFF    // Low Voltage Programming Enable bit->HV on MCLR/VPP must be used for programming

// CONFIG5L
#pragma config CP = OFF    // PFM and Data EEPROM Code Protection bit->PFM and Data EEPROM code protection disabled

#include "pic16f_platform.h"

// Using the XC8 compiler like so worked?
// xc8-cc -mcpu=16F877A -Ilib/PS2Plus -DPLATFORM_PIC16F ./src/main.c ./lib/PS2Plus/commands/button_inclusions_command.c ./lib/PS2Plus/commands/command.c ./lib/PS2Plus/commands/configur e_analog_response_command.c ./lib/PS2Plus/commands/device_descriptor_46_command.c ./lib/PS2Plus/commands/device_descriptor_47_command.c ./lib/PS2Plus/commands/device_descriptor_4c_com mand.c ./lib/PS2Plus/commands/initialize_pressure_sensor_command.c ./lib/PS2Plus/commands/main_polling_command.c ./lib/PS2Plus/commands/map_motors_command.c ./lib/PS2Plus/commands/sta tus_info_command.c ./lib/PS2Plus/commands/switch_digital_analog_mode_command.c ./lib/PS2Plus/controller_input.c ./lib/PS2Plus/controller_state.c ./lib/PS2Plus/platforms/arduino.c ./li b/PS2Plus/platforms/native.c ./lib/PS2Plus/platforms/pic16f.c ./lib/PS2Plus/utilities/debounced_bool.c ./lib/PS2Plus/utilities/latching_bool.c

// Interrupt callback to the main program loop
platform_interrupt_callback main_loop_callback = NULL;

void INTERRUPT_Initialize(void) {
  INTCON0bits.GIEH = 1;
  INTCON0bits.GIEL = 1;
  INTCON0bits.IPEN = 1;
  
  // Assign peripheral interrupt priority vectors
//  IPR3bits.TMR0IP = 1;
//  IPR2bits.SPI1IP = 1;
  IPR2bits.SPI1RXIP = 1;
}

void OSCILLATOR_Initialize(void)
{
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
}

void PMD_Initialize(void)
{
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

void platform_init(platform_interrupt_callback callback) {
  main_loop_callback = callback;
  
  INTERRUPT_Initialize();
  PMD_Initialize();
  OSCILLATOR_Initialize();
  
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