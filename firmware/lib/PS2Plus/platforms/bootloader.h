#ifndef PLATFORMS_BOOTLOADER_H
#define PLATFORMS_BOOTLOADER_H

#include <stddef.h>
#include <stdint.h>
#include <shared/bootloader.h>

/**
 * @brief Function pointer type used to point to the start address of the firmware
 */
typedef void (*platform_bootloader_execute_firmware_function)(void);

/**
 * @brief Transfer execution from the bootloader to the firmware
 */
void platform_bootloader_execute_firmware(void);

/*
 * @brief Returns true if the update record has a valid target address
 * 
 * If the target address falls into the bootloader address range, this will return false.
 */
bool platform_bootloader_validate_update_record_address(ps2plus_bootloader_update_record *record);

/**
 * @brief Flashes the update record to program memory
 */
bool platform_bootloader_flash_update_record(ps2plus_bootloader_update_record *record);

/**
 * @brief Calculates the checksum for the current loaded firmware
 */
uint8_t platform_bootloader_calculate_firmware_checksum();

#endif /* PLATFORMS_MEMORY_H */