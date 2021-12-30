#include <commands/command.h>
#include <unity.h>

#include "command_helper.h"

/**
 * @brief Global controller state used by the command processors
 */
extern controller_state state;

void test_status_info_command_digital_mode();
void test_status_info_command_analog_mode();
void test_status_info_command_analog_full_mode();
void test_device_descriptor_46_command_first_byte();
void test_device_descriptor_46_command_second_byte();
void test_device_descriptor_47_command_only_byte();
void test_device_descriptor_4c_command_first_byte();
void test_device_descriptor_4c_command_second_byte();
void test_switch_digital_analog_mode_command_set_digital();
void test_switch_digital_analog_mode_command_set_analog();
void test_switch_digital_analog_mode_command_set_analog_with_lock();
void test_switch_digital_analog_mode_command_toggling_removes_pressures();