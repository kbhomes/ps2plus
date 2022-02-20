#ifdef PS2PLUS_FIRMWARE

#include "custom_config.h"

#include <platforms/platform.h>

#define CONFIGURATION_ADDRESS_VERSION 0x00
#define CONFIGURATION_ADDRESS_START sizeof(uint16_t)
#define CONFIGURATION_ADDRESS(FIELD) (CONFIGURATION_ADDRESS_START + CONFIGURATION_ID(FIELD))

void controller_custom_config_initialize(controller_custom_config *config) {
  config->enabled = true;
  config->configuration_version = 0;
  
  primitive_data_initialize_boolean(&config->values.enable_button_remapping, false);
  for (uint8_t b = 0; b < NUM_DIGITAL_BUTTONS; b++) {
    primitive_data_initialize_uint8(&config->values.button_remapping[b], b);
  }

  primitive_data_initialize_boolean(&config->values.enable_joystick_axis_range_remapping, false);
  for (size_t a = 0; a < NUM_JOYSTICK_AXIS_RANGES; a++) {
    primitive_data_initialize_uint8(&config->values.joystick_axis_range_remapping[a], (
      (a % 3 == 0) ? 0x00 :
      (a % 3 == 1) ? 0x7F :
      (a % 3 == 2) ? 0xFF : 
      0x00));
  }
  
  primitive_data_initialize_uint8(&config->values.joystick_deadzone_left, 0x00);
  primitive_data_initialize_uint8(&config->values.joystick_deadzone_right, 0x00);
  
  primitive_data_initialize_boolean(&config->values.joystick_digital_enable_left, false);
  primitive_data_initialize_boolean(&config->values.joystick_digital_enable_right, false);
  primitive_data_initialize_uint8(&config->values.joystick_digital_threshold_left, 0x40);
  primitive_data_initialize_uint8(&config->values.joystick_digital_threshold_left, 0x40);
}

void controller_custom_config_load(controller_custom_config *config) {
  config->configuration_version = platform_memory_read_short(CONFIGURATION_ADDRESS_VERSION);
  
  config->values.enable_button_remapping.boolean = platform_memory_read(CONFIGURATION_ADDRESS(enable_button_remapping));
  for (uint8_t b = 0; b < NUM_DIGITAL_BUTTONS; b++) {
    config->values.button_remapping[b].uint8 = platform_memory_read(CONFIGURATION_ADDRESS(button_remapping) + b);
  }
  
  config->values.enable_joystick_axis_range_remapping.boolean = platform_memory_read(CONFIGURATION_ADDRESS(enable_joystick_axis_range_remapping));
  for (size_t a = 0; a < NUM_JOYSTICK_AXIS_RANGES; a++) {
    config->values.joystick_axis_range_remapping[a].uint8 = platform_memory_read(CONFIGURATION_ADDRESS(joystick_axis_range_remapping) + a);
  }
  
  config->values.joystick_deadzone_left.uint8 = platform_memory_read(CONFIGURATION_ADDRESS(joystick_deadzone_left));
  config->values.joystick_deadzone_right.uint8 = platform_memory_read(CONFIGURATION_ADDRESS(joystick_deadzone_right));
  
  config->values.joystick_digital_enable_left.boolean = platform_memory_read(CONFIGURATION_ADDRESS(joystick_digital_enable_left));
  config->values.joystick_digital_enable_right.boolean = platform_memory_read(CONFIGURATION_ADDRESS(joystick_digital_enable_right));
  config->values.joystick_digital_threshold_left.uint8 = platform_memory_read(CONFIGURATION_ADDRESS(joystick_digital_threshold_left));
  config->values.joystick_digital_threshold_left.uint8 = platform_memory_read(CONFIGURATION_ADDRESS(joystick_digital_threshold_left));
}

void controller_custom_config_save(controller_custom_config *config) {
  platform_memory_write_short(CONFIGURATION_ADDRESS_VERSION, config->configuration_version);

  platform_memory_write(CONFIGURATION_ADDRESS(enable_button_remapping), config->values.enable_button_remapping.boolean);
  for (uint8_t b = 0; b < NUM_DIGITAL_BUTTONS; b++) {
    platform_memory_write(CONFIGURATION_ADDRESS(button_remapping) + b, config->values.button_remapping[b].uint8);
  }
  
  platform_memory_write(CONFIGURATION_ADDRESS(enable_joystick_axis_range_remapping), config->values.enable_joystick_axis_range_remapping.boolean);
  for (size_t a = 0; a < NUM_JOYSTICK_AXIS_RANGES; a++) {
    platform_memory_write(CONFIGURATION_ADDRESS(joystick_axis_range_remapping) + a, config->values.joystick_axis_range_remapping[a].uint8);
  }
  
  platform_memory_write(CONFIGURATION_ADDRESS(joystick_deadzone_left), config->values.joystick_deadzone_left.uint8);
  platform_memory_write(CONFIGURATION_ADDRESS(joystick_deadzone_right), config->values.joystick_deadzone_right.uint8);
  
  platform_memory_write(CONFIGURATION_ADDRESS(joystick_digital_enable_left), config->values.joystick_digital_enable_left.boolean);
  platform_memory_write(CONFIGURATION_ADDRESS(joystick_digital_enable_right), config->values.joystick_digital_enable_right.boolean);
  platform_memory_write(CONFIGURATION_ADDRESS(joystick_digital_threshold_left), config->values.joystick_digital_threshold_left.uint8);
  platform_memory_write(CONFIGURATION_ADDRESS(joystick_digital_threshold_left), config->values.joystick_digital_threshold_left.uint8);
}

#endif