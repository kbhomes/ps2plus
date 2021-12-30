#include "test_commands.h"

#include <platform.h>
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
  platform_init();

  UNITY_BEGIN();
  RUN_TEST(test_initialize_pressure_sensor_command_constant_response);
  RUN_TEST(test_button_inclusions_command_digital_mode);
  RUN_TEST(test_button_inclusions_command_analog_mode);
  RUN_TEST(test_button_inclusions_command_analog_full_mode);
  RUN_TEST(test_main_polling_command_digital_mode);
  RUN_TEST(test_main_polling_command_analog_mode);
  RUN_TEST(test_main_polling_command_analog_full_mode);
  RUN_TEST(test_main_polling_command_with_motors);
  RUN_TEST(test_switch_digital_analog_mode_command_set_digital);
  RUN_TEST(test_switch_digital_analog_mode_command_set_analog);
  RUN_TEST(test_switch_digital_analog_mode_command_set_analog_with_lock);
  RUN_TEST(test_switch_digital_analog_mode_command_toggling_removes_pressures);
  RUN_TEST(test_status_info_command_digital_mode);
  RUN_TEST(test_status_info_command_analog_mode);
  RUN_TEST(test_status_info_command_analog_full_mode);
  RUN_TEST(test_device_descriptor_46_command_first_byte);
  RUN_TEST(test_device_descriptor_46_command_second_byte);
  RUN_TEST(test_device_descriptor_47_command_only_byte);
  RUN_TEST(test_device_descriptor_4c_command_first_byte);
  RUN_TEST(test_device_descriptor_4c_command_second_byte);
  RUN_TEST(test_map_motors_command_small_motor);
  RUN_TEST(test_map_motors_command_large_motor);
  RUN_TEST(test_map_motors_command_both_motors);
  RUN_TEST(test_map_motors_command_no_motors);
  RUN_TEST(test_map_motors_command_retains_mappings);
  RUN_TEST(test_configure_analog_response_command_digital_mode);
  RUN_TEST(test_configure_analog_response_command_analog_mode);
  RUN_TEST(test_configure_analog_response_command_analog_full_mode);
  UNITY_END();
}