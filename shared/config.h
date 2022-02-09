#ifndef SHARED_CONFIG_H
#define SHARED_CONFIG_H

#include "controller.h"
#include "primitive_data.h"

/**
 * @brief Collection of all configuration values, each being a `primitive_data` which 
 *        will be serialized for transfer between the firmware and the PS2+ configurator.
 * 
 * NOTE: All configuration values below are stored in internal storage in the order
 *       in which they appear. Do not change the order of these configurations
 *       without bumping the configuration version!
 */
typedef struct {
  /**
   * @brief Min, center, and max remapping values for each joystick axis
   */
  primitive_data /* uint8 */ joystick_axis_range_remapping_values[NUM_JOYSTICK_AXIS_RANGES];

  /**
   * @brief When enabled and the controller is in digital mode, the left and right joysticks
   *        simulate the D-pad and face buttons respectively
   */
  primitive_data /* boolean */ joystick_digital_mode_enabled;

  /**
   * @brief When enabled, the cross and circle buttons are swapped
   */
  primitive_data /* boolean */ global_button_remapping_enabled;
} ps2plus_configuration;

#define NUM_CUSTOM_CONFIGURATIONS (sizeof(ps2plus_configuration) / sizeof(primitive_data))
#define CONFIGURATION_ID(FIELD) ((uint16_t)(offsetof(ps2plus_configuration, FIELD) / sizeof(primitive_data)))

#endif /* SHARED_CONFIG_H */