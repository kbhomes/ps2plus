#include "custom_config.h"

#include <platforms/platform.h>

#define CONFIGURATION_ADDRESS_VERSION 0x00
#define CONFIGURATION_ADDRESS_START sizeof(uint16_t)
#define CONFIGURATION_ADDRESS(FIELD) (CONFIGURATION_ADDRESS_START + offsetof(controller_custom_config_values, FIELD))

void controller_custom_config_initialize(controller_custom_config *config) {
  config->enabled = true;
  config->configuration_version = 0;

  for (size_t a = 0; a < NUM_JOYSTICK_AXIS_RANGES; a++) {
    primitive_data_initialize_uint8(&config->values.joystick_axis_range_remapping_values[a], (
      (a % 3 == 0) ? 0x00 :
      (a % 3 == 1) ? 0x7F :
      (a % 3 == 2) ? 0xFF : 
      0x00));
  }
  primitive_data_initialize_boolean(&config->values.joystick_digital_mode_enabled, false);
  primitive_data_initialize_boolean(&config->values.global_button_remapping_enabled, false);
}

void controller_custom_config_load(controller_custom_config *config) {
  config->configuration_version = platform_memory_read_short(CONFIGURATION_ADDRESS_VERSION);

  for (size_t a = 0; a < NUM_JOYSTICK_AXIS_RANGES; a++) {
    config->values.joystick_axis_range_remapping_values[a].data.uint8 = platform_memory_read(CONFIGURATION_ADDRESS(joystick_axis_range_remapping_values) + a);
  }
  config->values.joystick_digital_mode_enabled.data.boolean = platform_memory_read(CONFIGURATION_ADDRESS(joystick_digital_mode_enabled));
  config->values.global_button_remapping_enabled.data.boolean = platform_memory_read(CONFIGURATION_ADDRESS(global_button_remapping_enabled));
}

void controller_custom_config_save(controller_custom_config *config) {
  platform_memory_write_short(CONFIGURATION_ADDRESS_VERSION, config->configuration_version);
  for (size_t a = 0; a < NUM_JOYSTICK_AXIS_RANGES; a++) {
    platform_memory_write(CONFIGURATION_ADDRESS(joystick_axis_range_remapping_values), config->values.joystick_axis_range_remapping_values[a].data.uint8);
  }
  platform_memory_write(CONFIGURATION_ADDRESS(joystick_digital_mode_enabled), config->values.joystick_digital_mode_enabled.data.boolean);
  platform_memory_write(CONFIGURATION_ADDRESS(global_button_remapping_enabled), config->values.global_button_remapping_enabled.data.boolean);
}