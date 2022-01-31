#include "command.h"

/**
 * @brief Processors for PlayStation 2 controller commands
 * 
 * All controller commands that can be processed have a 4 as the higher nibble. The correct
 * command processor can be obtained by taking the lower nibble of the command ID and 
 * indexing into this array.
 */
command_processor *command_controller_processors[] = {
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
};

/**
 * @brief Processors for PS2+ configuration commands
 * 
 * PS2+ supports custom commands that are sent by the PS2+ configurator console application.
 * These commands get PS2+ status information, get/set configuration values, and enable
 * firmware updates. These commands have a 7 as the higher nibble.
 */
command_processor *command_ps2plus_processors[] = {
  &command_ps2plus_get_version,                    // 70h
  &command_ps2plus_get_configuration,              // 71h
  &command_ps2plus_set_configuration,              // 72h
  &command_ps2plus_disable_enable_configuration,   // 73h
  NULL, /* &command_ps2plus_restore_configuration_defaults, */ // 74h
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
  NULL, /* &command_ps2plus_reboot_controller, */              // 7Dh
  NULL, /* &command_ps2plus_update_firmware_data, */           // 7Eh
  NULL, /* &command_ps2plus_query_firmware_update_status, */   // 7Fh
};

/**
 * @brief Two-dimensional table of polling commands indexed first by the upper nibble and
 *        then by the lower nibble of the command ID.
 */
command_processor **command_processors[] = {
  NULL, NULL, NULL, NULL,
  &command_controller_processors,                               // 40h - 4Fh
  NULL, NULL,
  &command_ps2plus_processors,                                  // 70h - 7Fh
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

command_processor *command_find_processor(uint8_t id) {
  // Identify the indices into the command processor table
  uint8_t upper_nibble = id >> 4;
  uint8_t lower_nibble = id & 0x0F;

  // Return the command processor for this command
  command_processor **processors = command_processors[upper_nibble];
  if (processors) {
    return processors[lower_nibble];
  }

  return NULL;
}