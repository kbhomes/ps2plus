#include "test_commands.h"

#define MM_COMMAND_ID 0x4D
#define MM_PAYLOAD_SIZE 6

const uint8_t MM_PAYLOAD_MAP_SMALL_MOTOR[MM_PAYLOAD_SIZE] = { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
const uint8_t MM_PAYLOAD_MAP_LARGE_MOTOR[MM_PAYLOAD_SIZE] = { 0xFF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF };
const uint8_t MM_PAYLOAD_MAP_BOTH_MOTORS[MM_PAYLOAD_SIZE] = { 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF };
const uint8_t MM_PAYLOAD_MAP_NO_MOTORS[MM_PAYLOAD_SIZE]   = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

void test_command_controller_map_motors_small_motor() {
  uint8_t *actual_output = helper_run_command(&state, MM_COMMAND_ID, MM_PAYLOAD_MAP_SMALL_MOTOR, MM_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(MM_PAYLOAD_MAP_NO_MOTORS, actual_output, MM_PAYLOAD_SIZE, "Motors should initially be unmapped");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x00, state.rumble_motor_small.mapping, "Small motor should be mapped");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xFF, state.rumble_motor_large.mapping, "Large motor should be unmapped");
}

void test_command_controller_map_motors_large_motor() {
  uint8_t *actual_output = helper_run_command(&state, MM_COMMAND_ID, MM_PAYLOAD_MAP_LARGE_MOTOR, MM_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(MM_PAYLOAD_MAP_NO_MOTORS, actual_output, MM_PAYLOAD_SIZE, "Motors should initially be unmapped");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xFF, state.rumble_motor_small.mapping, "Small motor should be unmapped");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x01, state.rumble_motor_large.mapping, "Large motor should be mapped");
}

void test_command_controller_map_motors_both_motors() {
  uint8_t *actual_output = helper_run_command(&state, MM_COMMAND_ID, MM_PAYLOAD_MAP_BOTH_MOTORS, MM_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(MM_PAYLOAD_MAP_NO_MOTORS, actual_output, MM_PAYLOAD_SIZE, "Motors should initially be unmapped");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x00, state.rumble_motor_small.mapping, "Small motor should be mapped");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x01, state.rumble_motor_large.mapping, "Large motor should be mapped");
}

void test_command_controller_map_motors_no_motors() {
  uint8_t *actual_output = helper_run_command(&state, MM_COMMAND_ID, MM_PAYLOAD_MAP_NO_MOTORS, MM_PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(MM_PAYLOAD_MAP_NO_MOTORS, actual_output, MM_PAYLOAD_SIZE, "Motors should initially be unmapped");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xFF, state.rumble_motor_small.mapping, "Small motor should be unmapped");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xFF, state.rumble_motor_large.mapping, "Large motor should be unmapped");
}

void test_command_controller_map_motors_retains_mappings() {
  // Process two consecutive motor mapping commands
  uint8_t *first_output = helper_run_command(&state, MM_COMMAND_ID, MM_PAYLOAD_MAP_BOTH_MOTORS, MM_PAYLOAD_SIZE);
  uint8_t *second_output = helper_run_command(&state, MM_COMMAND_ID, MM_PAYLOAD_MAP_SMALL_MOTOR, MM_PAYLOAD_SIZE);

  // Each new command should output the controller's previous motor mappings
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(MM_PAYLOAD_MAP_NO_MOTORS, first_output, MM_PAYLOAD_SIZE, "Motors should be unmapped before first command");
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(MM_PAYLOAD_MAP_BOTH_MOTORS, second_output, MM_PAYLOAD_SIZE, "Motors should both be mapped before second command");
}