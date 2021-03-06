#include "command.h"

/**
 * @brief Processors for PlayStation 2 controller commands
 * 
 * All controller commands that can be processed have a 4 as the higher nibble. The correct
 * command processor can be obtained by taking the lower nibble of the command ID and 
 * indexing into this array.
 */
const command_processor *command_controller_processors[] = {
#if defined(PS2PLUS_FIRMWARE)
  &command_controller_initialize_pressure_sensor,    // 40h
  &command_controller_button_inclusions,             // 41h
  &command_controller_main_polling,                  // 42h
  &command_controller_enter_exit_config,             // 43h
  &command_controller_switch_digital_analog_mode,    // 44h
  &command_controller_status_info,                   // 45h
  &command_controller_device_descriptor_46,          // 46h
  &command_controller_device_descriptor_47,          // 47h
  NULL, NULL, NULL, NULL,
  &command_controller_device_descriptor_4c,          // 4Ch
  &command_controller_map_motors,                    // 4Dh
  NULL,
  &command_controller_configure_analog_response,     // 4Fh
#elif defined(PS2PLUS_BOOTLOADER)
  NULL, NULL,
  &command_controller_main_polling,                  // 42h
  NULL, NULL, NULL, NULL, 
  NULL, NULL, NULL, NULL, 
  NULL, NULL, NULL, NULL, 
  NULL,
#endif
};

/**
 * @brief Processors for PS2+ configuration commands
 * 
 * PS2+ supports custom commands that are sent by the PS2+ configurator console application.
 * These commands get PS2+ status information, get/set configuration values, and enable
 * firmware updates. These commands have a 7 as the higher nibble.
 */
const command_processor *command_ps2plus_processors[] = {
#if defined(PS2PLUS_FIRMWARE)
  // Firmware requires support for all configuration-related commands
  &command_ps2plus_get_version,                    // 70h
  &command_ps2plus_get_configuration,              // 71h
  &command_ps2plus_set_configuration,              // 72h
  &command_ps2plus_disable_enable_configuration,   // 73h
  &command_ps2plus_restore_configuration_defaults, // 74h
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 75h - 7Ch
  &command_ps2plus_reboot_controller,              // 7Dh
  NULL, NULL,                                      // 7Eh - 7Fh
#elif defined(PS2PLUS_BOOTLOADER)
  // Bootloader only needs to support version and bootloader-specific commands
  &command_ps2plus_get_version,                                                  // 70h
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  // 71h - 7Dh
  &command_ps2plus_bootloader_update_firmware_data,                              // 7Eh
  &command_ps2plus_bootloader_query_firmware_update_status,                      // 7Fh
#endif
};

/**
 * @brief Two-dimensional table of polling commands indexed first by the upper nibble and
 *        then by the lower nibble of the command ID.
 */
const command_processor **command_processors[] = {
  NULL, NULL, NULL, NULL,
  command_controller_processors,                               // 40h - 4Fh
  NULL, NULL,
  command_ps2plus_processors,                                  // 70h - 7Fh
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

const command_processor *command_find_processor(uint8_t id) {     
  // Identify the indices into the command processor table
  uint8_t upper_nibble = id >> 4;
  uint8_t lower_nibble = id & 0x0F;

  // Return the command processor for this command
  const command_processor **processors = command_processors[upper_nibble];
  if (processors) {
    return processors[lower_nibble];
  }

  return NULL;
}