#ifndef TEST_COMMANDS_SUITE_H
#define TEST_COMMANDS_SUITE_H

#include <external/unity/unity.h>
#include <commands/command.h>
#include <shared/versions.h>

#include "command_helper.h"
#include "polling_helper.h"

#define TEST_VERSION_FIRMWARE            VERSION64(1, 2, 3, VERSION_METADATA_DEV)
#define TEST_VERSION_MICROCONTROLLER     "test/commands"
#define TEST_VERSION_CONFIGURATION       0xABCD
#define TEST_VERSION_BOOTLOADER          VERSION64(4, 5, 6, VERSION_METADATA_LATEST)

/**
 * @brief Global controller state used by the command processors
 */
extern controller_state state;

void test_command_controller_initialize_pressure_sensor_constant_response();
void test_command_controller_button_inclusions_digital_mode();
void test_command_controller_button_inclusions_analog_mode();
void test_command_controller_button_inclusions_analog_full_mode();
void test_command_controller_main_polling_digital_mode();
void test_command_controller_main_polling_analog_mode();
void test_command_controller_main_polling_analog_full_mode();
void test_command_controller_main_polling_with_motors();
void test_command_controller_enter_exit_config_digital_mode_polling();
void test_command_controller_enter_exit_config_analog_mode_polling();
void test_command_controller_enter_exit_config_analog_full_mode_polling();
void test_command_controller_enter_exit_config_enter_config();
void test_command_controller_enter_exit_config_exit_config();
void test_command_controller_switch_digital_analog_mode_set_digital();
void test_command_controller_switch_digital_analog_mode_set_analog();
void test_command_controller_switch_digital_analog_mode_set_analog_with_lock();
void test_command_controller_switch_digital_analog_mode_toggling_removes_pressures();
void test_command_controller_status_info_digital_mode();
void test_command_controller_status_info_analog_mode();
void test_command_controller_status_info_analog_full_mode();
void test_command_controller_device_descriptor_46_first_byte();
void test_command_controller_device_descriptor_46_second_byte();
void test_command_controller_device_descriptor_47_only_byte();
void test_command_controller_device_descriptor_4c_first_byte();
void test_command_controller_device_descriptor_4c_second_byte();
void test_command_controller_map_motors_small_motor();
void test_command_controller_map_motors_large_motor();
void test_command_controller_map_motors_both_motors();
void test_command_controller_map_motors_no_motors();
void test_command_controller_map_motors_retains_mappings();
void test_command_controller_configure_analog_response_digital_mode();
void test_command_controller_configure_analog_response_analog_mode();
void test_command_controller_configure_analog_response_analog_full_mode();

void test_command_ps2plus_get_version_firmware();
void test_command_ps2plus_get_version_microcontroller();
void test_command_ps2plus_get_version_configuration();
void test_command_ps2plus_get_configuration_valid();
void test_command_ps2plus_get_configuration_unknown();
void test_command_ps2plus_set_configuration_valid();
void test_command_ps2plus_set_configuration_unknown();
void test_command_ps2plus_disable_enable_configuration();
void test_command_ps2plus_disable_restore_configuration_defaults();

#endif