#ifdef PLATFORM_PIC16F

#include "pic16f_platform.h"

uint8_t platform_memory_read(size_t address) {
//  return eeprom_read_byte((uint8_t *)address);
    return 0;
}

void platform_memory_write(size_t address, uint8_t byte) {
//  eeprom_update_byte((uint8_t *)address, byte);
}

#endif /* PLATFORM_PIC16F */