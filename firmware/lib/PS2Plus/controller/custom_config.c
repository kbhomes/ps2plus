#include "custom_config.h"

#include <platforms/platform.h>

#define CONFIGURATION_ADDRESS_START 0x00
#define CONFIGURATION_ADDRESS(FIELD) \
          (CONFIGURATION_ADDRESS_START + offsetof(controller_custom_config, FIELD) - offsetof(controller_custom_config, configuration_version))

void controller_custom_config_initialize(controller_custom_config *config) {
  config->enabled = true;
  config->configuration_version = 0;
  config->joystick_digital_mode_enabled = false;
  config->global_button_remapping_enabled = false;

  for (size_t a = 0; a < NUM_JOYSTICK_AXIS_RANGES; a++) {
    config->joystick_axis_range_remapping_values[a] = (
      (a % 3 == 0) ? 0x00 :
      (a % 3 == 1) ? 0x7F :
      (a % 3 == 2) ? 0xFF : 0x00);
  }
}

void controller_custom_config_load(controller_custom_config *config) {
  config->configuration_version = platform_memory_read_short(CONFIGURATION_ADDRESS(configuration_version));
  platform_memory_read_array(CONFIGURATION_ADDRESS(joystick_axis_range_remapping_values), config->joystick_axis_range_remapping_values, NUM_JOYSTICK_AXIS_RANGES);
  config->joystick_digital_mode_enabled = platform_memory_read(CONFIGURATION_ADDRESS(joystick_digital_mode_enabled));
  config->joystick_digital_mode_enabled = platform_memory_read(CONFIGURATION_ADDRESS(joystick_digital_mode_enabled));
}

void controller_custom_config_save(controller_custom_config *config) {
  platform_memory_write_short(CONFIGURATION_ADDRESS(configuration_version), config->configuration_version);
  platform_memory_write_array(CONFIGURATION_ADDRESS(joystick_axis_range_remapping_values), config->joystick_axis_range_remapping_values, NUM_JOYSTICK_AXIS_RANGES);
  platform_memory_write(CONFIGURATION_ADDRESS(joystick_digital_mode_enabled), config->joystick_digital_mode_enabled);
  platform_memory_write(CONFIGURATION_ADDRESS(global_button_remapping_enabled), config->global_button_remapping_enabled);
}