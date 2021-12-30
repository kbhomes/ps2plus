#include <commands/command.h>
#include <unity.h>

#include "command_helper.h"
#include "polling_helper.h"

/**
 * @brief Global controller state used by the command processors
 */
extern controller_state state;

void test_initialize_pressure_sensor_command_constant_response();
void test_button_inclusions_command_digital_mode();
void test_button_inclusions_command_analog_mode();
void test_button_inclusions_command_analog_full_mode();
void test_main_polling_command_digital_mode();
void test_main_polling_command_analog_mode();
void test_main_polling_command_analog_full_mode();
void test_main_polling_command_with_motors();
void test_enter_exit_config_command_digital_mode_polling();
void test_enter_exit_config_command_analog_mode_polling();
void test_enter_exit_config_command_analog_full_mode_polling();
void test_enter_exit_config_command_enter_config();
void test_enter_exit_config_command_exit_config();
void test_switch_digital_analog_mode_command_set_digital();
void test_switch_digital_analog_mode_command_set_analog();
void test_switch_digital_analog_mode_command_set_analog_with_lock();
void test_switch_digital_analog_mode_command_toggling_removes_pressures();
void test_status_info_command_digital_mode();
void test_status_info_command_analog_mode();
void test_status_info_command_analog_full_mode();
void test_device_descriptor_46_command_first_byte();
void test_device_descriptor_46_command_second_byte();
void test_device_descriptor_47_command_only_byte();
void test_device_descriptor_4c_command_first_byte();
void test_device_descriptor_4c_command_second_byte();
void test_map_motors_command_small_motor();
void test_map_motors_command_large_motor();
void test_map_motors_command_both_motors();
void test_map_motors_command_no_motors();
void test_map_motors_command_retains_mappings();
void test_configure_analog_response_command_digital_mode();
void test_configure_analog_response_command_analog_mode();
void test_configure_analog_response_command_analog_full_mode();