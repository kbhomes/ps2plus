#ifndef CONTROLLER_CUSTOM_CONFIG_H
#define CONTROLLER_CUSTOM_CONFIG_H

#include <stdbool.h>
#include <stdint.h>

#include <utilities/primitive_data.h>

typedef enum {
  JSAxisRange_LeftXMin,
  JSAxisRange_LeftXCenter,
  JSAxisRange_LeftXMax,
  JSAxisRange_LeftYMin,
  JSAxisRange_LeftYCenter,
  JSAxisRange_LeftYMax,
  JSAxisRange_RightXMin,
  JSAxisRange_RightXCenter,
  JSAxisRange_RightXMax,
  JSAxisRange_RightYMin,
  JSAxisRange_RightYCenter,
  JSAxisRange_RightYMax,
  NUM_JOYSTICK_AXIS_RANGES,
} controller_joystick_axis_range_remapping;

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
} controller_custom_config_values;

#define NUM_CUSTOM_CONFIGURATIONS (sizeof(controller_custom_config_values) / sizeof(primitive_data))

typedef struct {
  /**
   * @brief Whether custom configurations are globally enabled (not stored in storage)
   */
  bool enabled;

  /**
   * @brief Configuration version as saved in internal storage. On startup, a mismatch
   *        between the firmware's configuration version and the stored version indicates
   *        a backwards-incompatible configuration change that will require resetting. 
   */
  uint16_t configuration_version;

  /**
   * @brief Configuration values that can be serialized for transfer between the firmware
   *        and the PS2+ configurator application
   */
  union {
    controller_custom_config_values values;
    primitive_data all[NUM_CUSTOM_CONFIGURATIONS];
  };
} controller_custom_config;

void controller_custom_config_initialize(controller_custom_config *config);
void controller_custom_config_load(controller_custom_config *config);
void controller_custom_config_save(controller_custom_config *config);

#endif /* CONTROLLER_CUSTOM_CONFIG_H */