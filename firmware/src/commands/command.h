#ifndef COMMANDS_COMMAND_H
#define COMMANDS_COMMAND_H

#include <controller/state.h>
#include <platforms/platform.h>

#include "packet.h"

typedef enum {
  CRInitialized,
  CRProcessing,
  CRCompleted,
  CRError,
} command_result;

typedef command_result (*command_processor_initialize_func)(volatile command_packet *packet, controller_state *);
typedef command_result (*command_processor_process_func)(volatile command_packet *packet, controller_state *);
typedef void (*command_processor_finalize_func)(volatile command_packet *packet, controller_state *);

typedef struct command_processor {
  uint8_t id;
  command_processor_initialize_func initialize;
  command_processor_process_func process;
  command_processor_finalize_func finalize;
} command_processor;

static void command_processor_finalize_dummy(volatile command_packet *packet, controller_state *state) {
  return;
}

// Controller command processors
#if defined(PS2PLUS_FIRMWARE)
extern const command_processor command_controller_initialize_pressure_sensor;
extern const command_processor command_controller_button_inclusions;
extern const command_processor command_controller_main_polling;
extern const command_processor command_controller_enter_exit_config;
extern const command_processor command_controller_switch_digital_analog_mode;
extern const command_processor command_controller_status_info;
extern const command_processor command_controller_device_descriptor_46;
extern const command_processor command_controller_device_descriptor_47;
extern const command_processor command_controller_device_descriptor_4c;
extern const command_processor command_controller_map_motors;
extern const command_processor command_controller_configure_analog_response;
#endif

// PS2+ configuration command processors
extern const command_processor command_ps2plus_get_version;
#if defined(PS2PLUS_FIRMWARE)
extern const command_processor command_ps2plus_get_configuration;
extern const command_processor command_ps2plus_set_configuration;
extern const command_processor command_ps2plus_disable_enable_configuration;
extern const command_processor command_ps2plus_restore_configuration_defaults;
extern const command_processor command_ps2plus_reboot_controller;
#elif defined(PS2PLUS_BOOTLOADER)
extern const command_processor command_ps2plus_bootloader_update_firmware_data;
extern const command_processor command_ps2plus_bootloader_query_firmware_update_status;
#endif

/**
 * @brief Returns the command processor for the given command ID, or NULL if the 
 *        command ID is not supported.
 */
const command_processor *command_find_processor(uint8_t id);

#endif /* COMMANDS_COMMAND_H */