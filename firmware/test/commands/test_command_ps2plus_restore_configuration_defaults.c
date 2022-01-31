#include "test_commands.h"

#define PS2PLUS_RCD_COMMAND_ID 0x74

void test_command_ps2plus_disable_reset_configuration_defaults() {
  bool original_jdme = state.custom_config.values.joystick_digital_mode_enabled.boolean;
  bool original_gbre = state.custom_config.values.global_button_remapping_enabled.boolean;
  
  // Put non-default data in storage (flipping booleans in this case)
  state.custom_config.values.joystick_digital_mode_enabled.boolean = !original_jdme;
  state.custom_config.values.global_button_remapping_enabled.boolean = !original_gbre;
  controller_custom_config_save(&state.custom_config);

  // Execute the Reset Configuration Defaults command
  helper_run_command(&state, PS2PLUS_RCD_COMMAND_ID, NULL, 0);
  
  // Ensure the configuration was reset in storage by reloading before comparing
  controller_custom_config_load(&state.custom_config);
  TEST_ASSERT_EQUAL_MESSAGE(original_jdme, state.custom_config.values.joystick_digital_mode_enabled.boolean, "joystick_digital_mode_enabled not reset to default");
  TEST_ASSERT_EQUAL_MESSAGE(original_gbre, state.custom_config.values.global_button_remapping_enabled.boolean, "global_button_remapping_enabled not reset to default");
}