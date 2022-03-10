#ifdef PLATFORM_PIC18F46K42

#include "pic18f46k42_platform.h"

// Firmware "jump vector"
platform_bootloader_execute_firmware_function execute_firmware_function = PIC_FIRMWARE_BASE;

void platform_bootloader_execute_firmware(void) {
  // Disable all interrupts
  GIE = 0;
  
  // "Jump" to the firmware's start address
  execute_firmware_function();
}

bool platform_bootloader_validate_update_record_address(ps2plus_bootloader_update_record *record) {
  return record->target_address >= 0x4000 && record->target_address < 0x10000;
}

bool platform_bootloader_flash_update_record(ps2plus_bootloader_update_record *record) {
  
}

#endif /* PLATFORM_PIC18F46K42 */