#ifndef CONTROLLER_CUSTOM_CONFIG_H
#define CONTROLLER_CUSTOM_CONFIG_H

#include <stdbool.h>
#include <stdint.h>

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

// NOTE: All configuration values below are stored in internal storage in the order
// in which they appear below. Do not change the order of these configurations
// without bumping the configuration version of this firmware!

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
   * @brief Min, center, and max remapping values for each joystick axis
   */
  uint8_t joystick_axis_range_remapping_values[NUM_JOYSTICK_AXIS_RANGES];

  /**
   * @brief When enabled and the controller is in digital mode, the left and right joysticks
   *        simulate the D-pad and face buttons respectively
   */
  bool joystick_digital_mode_enabled;

  /**
   * @brief When enabled, the cross and circle buttons are swapped
   */
  bool global_button_remapping_enabled;
} controller_custom_config;

void controller_custom_config_initialize(controller_custom_config *config);
void controller_custom_config_load(controller_custom_config *config);
void controller_custom_config_save(controller_custom_config *config);

#endif /* CONTROLLER_CUSTOM_CONFIG_H */