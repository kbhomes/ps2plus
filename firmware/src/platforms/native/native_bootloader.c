#ifdef PLATFORM_NATIVE

#include "native_platform.h"

void platform_bootloader_execute_firmware(void) {
    
}

bool platform_bootloader_erase_firmware(void) {
    return false;
}

platform_bootloader_update_record_address_validity platform_bootloader_validate_update_record_address(ps2plus_bootloader_update_record *record) {
    return BLAddressInvalid;
}

bool platform_bootloader_flash_update_record(ps2plus_bootloader_update_record *record) {
    return false;
}

uint8_t platform_bootloader_calculate_firmware_checksum() {
    return 0;
}

#endif