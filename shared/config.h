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
  // Allows remapping any button to any other button. This is a boolean that enables/disables remapping, and 
  // a `NUM_DIGITAL_BUTTONS` length byte array: indices are the input buttons and the values are the remapped 
  // buttons. To prevent accidentally remapping the buttons into an unusable state, the controller’s firmware 
  // should have a button combination/sequence to reset the internal button configuration.
  primitive_data /* bool */ enable_button_remapping;
  primitive_data /* uint8 */ button_remapping[NUM_DIGITAL_BUTTONS];

  // Allows scaling each joystick’s true ranges to the full range expected by the PS2. Additionally helps to 
  // counteract drift by remapping each joystick’s center. This is a boolean that enables/disables remapping, 
  // and a 12-length byte array. Each joystick axis has a minimum, center, and maximum value. By identifying 
  // the minimum, center, and maximum values of each joystick axis, the firmware can linearly map an axis’ 
  // `[minimum, center]` range to `[0, 127]` and `[center, maximum]` to `[127, 255]`.
  primitive_data /* bool */ enable_joystick_axis_range_remapping;
  primitive_data /* uint8 */ joystick_axis_range_remapping[NUM_JOYSTICK_AXIS_RANGES];

  // Allows specifying a deadzone on each joystick to decrease sensitivity radially around the center. This is one 
  // byte per joystick in the range `[0, 127]`. Deadzones are applied after axis range remapping (if enabled). For 
  // a deadzone value of `VDeadzone`, joystick axis values between `[127 - VDeadzone, 127 + VDeadzone]` will be 
  // reported as `127` to the console.
  primitive_data /* uint8 */ joystick_deadzone_left;
  primitive_data /* uint8 */ joystick_deadzone_right;

  // Allows using the joysticks to drive the D-pad and face buttons when the controller is in digital mode. This 
  // is one boolean and one byte per joystick: enable the left joystick to drive the D-pad, and enable the right 
  // joystick to drive the face buttons; and a threshold value between `[0, 127]` for each.  When in digital mode 
  // and the joystick is enabled, joystick inputs between `[0, 127 - threshold]` will register as Left/Square or 
  // Up/Triangle; joystick inputs between `[127 + threshold, 255]` will register as Right/Circle or Down/Cross.
  primitive_data /* bool */ joystick_digital_enable_left;
  primitive_data /* bool */ joystick_digital_enable_right;
  primitive_data /* uint8 */ joystick_digital_threshold_left;
  primitive_data /* uint8 */ joystick_digital_threshold_right;
} ps2plus_configuration;

#define NUM_CUSTOM_CONFIGURATIONS (sizeof(ps2plus_configuration) / sizeof(primitive_data))
#define CONFIGURATION_ID(FIELD) ((uint16_t)(offsetof(ps2plus_configuration, FIELD) / sizeof(primitive_data)))

#endif /* SHARED_CONFIG_H */