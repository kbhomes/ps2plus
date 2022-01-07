#include "command.h"

command_processor *command_processors[] = {
  &initialize_pressure_sensor_command,    // 40h
  &button_inclusions_command,             // 41h
  &main_polling_command,                  // 42h
  &enter_exit_config_command,             // 43h
  &switch_digital_analog_mode_command,    // 44h
  &status_info_command,                   // 45h
  &device_descriptor_46_command,          // 46h
  &device_descriptor_47_command,          // 47h
  NULL,
  NULL,
  NULL,
  NULL,
  &device_descriptor_4c_command,          // 4Ch
  &map_motors_command,                    // 4Dh
  NULL,
  &configure_analog_response_command,     // 4Fh
};

command_processor *command_find_processor(uint8_t id) {
  return command_processors[id & 0x0F];
}