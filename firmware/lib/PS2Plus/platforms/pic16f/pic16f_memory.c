#ifdef PLATFORM_PIC16F

#include "pic16f_platform.h"

size_t platform_memory_total_size() {
    // PIC18F46K42 have 1KB of EEPROM storage.
    return 0x400;
}

uint8_t platform_memory_read(size_t address) {
    // Setup EEPROM access and address selection
    NVMCON1 = 0; 
    NVMADRL = address & 0xFF;
    NVMADRH = (address >> 8) & 0x03;

    // Issue the EEPROM read
    NVMCON1bits.RD = 1;
    return NVMDAT;
}

void platform_memory_write(size_t address, uint8_t byte) {
    // Issue an initial EEPROM read so we can determine if an
    // erase-and-write cycle is necessary in the first place
    uint8_t current_value = platform_memory_read(address);
    if (current_value == byte) {
        return;
    }
    
    // Setup EEPROM access and address selection
    NVMCON1 = 0; 
    NVMADRL = address & 0xFF;
    NVMADRH = (address >> 8) & 0x03;

    // Prepare the value to be written
    NVMDAT = byte;
    NVMCON1bits.WREN = 1;

    // Perform the NVM unlock sequence and issue the EEPROM write
    bool interrupts_enabled = GIE;
    GIE = 0;
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    NVMCON1bits.WR = 1;
    GIE = interrupts_enabled;

    // Wait for the write operation to complete
    while (NVMCON1bits.WR);

    // Disable EEPROM writes
    NVMCON1bits.WREN = 0;
}

#endif /* PLATFORM_PIC16F */