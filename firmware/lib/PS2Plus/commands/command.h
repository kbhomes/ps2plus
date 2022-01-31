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

typedef struct command_processor {
  uint8_t id;
  command_result (*initialize)(volatile command_packet *packet, controller_state *);
  command_result (*process)(volatile command_packet *packet, controller_state *);
  command_result (*finalize)(volatile command_packet *packet, controller_state *);
} command_processor;

// Controller command processors
extern command_processor command_controller_initialize_pressure_sensor;
extern command_processor command_controller_button_inclusions;
extern command_processor command_controller_main_polling;
extern command_processor command_controller_enter_exit_config;
extern command_processor command_controller_switch_digital_analog_mode;
extern command_processor command_controller_status_info;
extern command_processor command_controller_device_descriptor_46;
extern command_processor command_controller_device_descriptor_47;
extern command_processor command_controller_device_descriptor_4c;
extern command_processor command_controller_map_motors;
extern command_processor command_controller_configure_analog_response;

// PS2+ configuration command processors
extern command_processor command_ps2plus_get_version;
extern command_processor command_ps2plus_get_configuration;
extern command_processor command_ps2plus_set_configuration;

/**
 * @brief Returns the command processor for the given command ID, or NULL if the 
 *        command ID is not supported.
 */
command_processor *command_find_processor(uint8_t id);

#endif /* COMMANDS_COMMAND_H */