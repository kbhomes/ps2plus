#include "suite.h"

void test_command_ps2plus_disable_restore_configuration_defaults() {
  // Testing two different config data types
  bool original_ebr = state.custom_config.values.enable_button_remapping.boolean;
  uint8_t original_jdtl = state.custom_config.values.joystick_digital_threshold_left.uint8;
  
  // Put non-default data in storage
  state.custom_config.values.enable_button_remapping.boolean = !original_ebr;
  state.custom_config.values.joystick_digital_threshold_left.uint8 = 0xFF - original_jdtl;
  controller_custom_config_save(&state.custom_config);

  // Execute the Restore Configuration Defaults command
  helper_run_command(command_ps2plus_restore_configuration_defaults.id, NULL, 0);
  
  // Ensure the configuration was reset in storage
  controller_custom_config_load(&state.custom_config);
  TEST_ASSERT_EQUAL_MESSAGE(original_ebr, state.custom_config.values.enable_button_remapping.boolean, "enable_button_remapping not reset to default");
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(original_jdtl, state.custom_config.values.joystick_digital_threshold_left.uint8, "joystick_digital_threshold_left not reset to default");
}