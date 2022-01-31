#include "test_commands.h"

#include <platforms/platform.h>
#include <stdio.h>
#include <unity.h>

controller_state state = { 
  .input = {
    .digital_buttons = { 0 },
    .joysticks = { 0 },
  },
  .analog_mode = CMDigital,
  .analog_mode_locked = false,
  .config_mode = false,
  .rumble_motor_small = { .mapping = 0xFF, .value = 0x00 },
  .rumble_motor_large = { .mapping = 0xFF, .value = 0x00 },
};

void setUp() {
  helper_initialize();
  controller_state_initialize(&state);
}

int main(void) {
  platform_init(NULL);

  UNITY_BEGIN();
  RUN_TEST(test_command_controller_initialize_pressure_sensor_constant_response);
  RUN_TEST(test_command_controller_button_inclusions_digital_mode);
  RUN_TEST(test_command_controller_button_inclusions_analog_mode);
  RUN_TEST(test_command_controller_button_inclusions_analog_full_mode);
  RUN_TEST(test_command_controller_main_polling_digital_mode);
  RUN_TEST(test_command_controller_main_polling_analog_mode);
  RUN_TEST(test_command_controller_main_polling_analog_full_mode);
  RUN_TEST(test_command_controller_main_polling_with_motors);
  RUN_TEST(test_command_controller_enter_exit_config_digital_mode_polling);
  RUN_TEST(test_command_controller_enter_exit_config_analog_mode_polling);
  RUN_TEST(test_command_controller_enter_exit_config_analog_full_mode_polling);
  RUN_TEST(test_command_controller_enter_exit_config_enter_config);
  RUN_TEST(test_command_controller_enter_exit_config_exit_config);
  RUN_TEST(test_command_controller_switch_digital_analog_mode_set_digital);
  RUN_TEST(test_command_controller_switch_digital_analog_mode_set_analog);
  RUN_TEST(test_command_controller_switch_digital_analog_mode_set_analog_with_lock);
  RUN_TEST(test_command_controller_switch_digital_analog_mode_toggling_removes_pressures);
  RUN_TEST(test_command_controller_status_info_digital_mode);
  RUN_TEST(test_command_controller_status_info_analog_mode);
  RUN_TEST(test_command_controller_status_info_analog_full_mode);
  RUN_TEST(test_command_controller_device_descriptor_46_first_byte);
  RUN_TEST(test_command_controller_device_descriptor_46_second_byte);
  RUN_TEST(test_command_controller_device_descriptor_47_only_byte);
  RUN_TEST(test_command_controller_device_descriptor_4c_first_byte);
  RUN_TEST(test_command_controller_device_descriptor_4c_second_byte);
  RUN_TEST(test_command_controller_map_motors_small_motor);
  RUN_TEST(test_command_controller_map_motors_large_motor);
  RUN_TEST(test_command_controller_map_motors_both_motors);
  RUN_TEST(test_command_controller_map_motors_no_motors);
  RUN_TEST(test_command_controller_map_motors_retains_mappings);
  RUN_TEST(test_command_controller_configure_analog_response_digital_mode);
  RUN_TEST(test_command_controller_configure_analog_response_analog_mode);
  RUN_TEST(test_command_controller_configure_analog_response_analog_full_mode);

  RUN_TEST(test_command_ps2plus_get_version_firmware);
  RUN_TEST(test_command_ps2plus_get_version_microcontroller);
  RUN_TEST(test_command_ps2plus_get_version_configuration);
  RUN_TEST(test_command_ps2plus_get_configuration_joystick_axis_range_remapping);
  RUN_TEST(test_command_ps2plus_get_configuration_joystick_digital_mode_enabled);
  RUN_TEST(test_command_ps2plus_get_configuration_global_button_remapping_enabled);
  RUN_TEST(test_command_ps2plus_get_configuration_unknown);
  RUN_TEST(test_command_ps2plus_set_configuration_joystick_axis_range_remapping);
  RUN_TEST(test_command_ps2plus_set_configuration_joystick_digital_mode_enabled);
  RUN_TEST(test_command_ps2plus_set_configuration_global_button_remapping_enabled);
  RUN_TEST(test_command_ps2plus_set_configuration_unknown);
  UNITY_END();
}