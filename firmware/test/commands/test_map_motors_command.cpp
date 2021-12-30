#include "test_commands.h"

const uint8_t COMMAND_ID = 0x4D;
const size_t PAYLOAD_SIZE = 6;

const uint8_t PAYLOAD_MAP_SMALL_MOTOR[PAYLOAD_SIZE] = { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
const uint8_t PAYLOAD_MAP_LARGE_MOTOR[PAYLOAD_SIZE] = { 0xFF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF };
const uint8_t PAYLOAD_MAP_BOTH_MOTORS[PAYLOAD_SIZE] = { 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF };
const uint8_t PAYLOAD_MAP_NO_MOTORS[PAYLOAD_SIZE]   = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

void test_map_motors_command_small_motor() {
  uint8_t *actual_output = helper_run_command(&state, COMMAND_ID, PAYLOAD_MAP_SMALL_MOTOR, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(PAYLOAD_MAP_NO_MOTORS, actual_output, PAYLOAD_SIZE, "Motors should initially be unmapped");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x00, state.rumble_motor_small.mapping, "Small motor should be mapped");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xFF, state.rumble_motor_large.mapping, "Large motor should be unmapped");
}

void test_map_motors_command_large_motor() {
  uint8_t *actual_output = helper_run_command(&state, COMMAND_ID, PAYLOAD_MAP_LARGE_MOTOR, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(PAYLOAD_MAP_NO_MOTORS, actual_output, PAYLOAD_SIZE, "Motors should initially be unmapped");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xFF, state.rumble_motor_small.mapping, "Small motor should be unmapped");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x01, state.rumble_motor_large.mapping, "Large motor should be mapped");
}

void test_map_motors_command_both_motors() {
  uint8_t *actual_output = helper_run_command(&state, COMMAND_ID, PAYLOAD_MAP_BOTH_MOTORS, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(PAYLOAD_MAP_NO_MOTORS, actual_output, PAYLOAD_SIZE, "Motors should initially be unmapped");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x00, state.rumble_motor_small.mapping, "Small motor should be mapped");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x01, state.rumble_motor_large.mapping, "Large motor should be mapped");
}

void test_map_motors_command_no_motors() {
  uint8_t *actual_output = helper_run_command(&state, COMMAND_ID, PAYLOAD_MAP_NO_MOTORS, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(PAYLOAD_MAP_NO_MOTORS, actual_output, PAYLOAD_SIZE, "Motors should initially be unmapped");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xFF, state.rumble_motor_small.mapping, "Small motor should be unmapped");
  TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xFF, state.rumble_motor_large.mapping, "Large motor should be unmapped");
}

void test_map_motors_command_retains_mappings() {
  // Process two consecutive motor mapping commands
  uint8_t *first_output = helper_run_command(&state, COMMAND_ID, PAYLOAD_MAP_BOTH_MOTORS, PAYLOAD_SIZE);
  uint8_t *second_output = helper_run_command(&state, COMMAND_ID, PAYLOAD_MAP_SMALL_MOTOR, PAYLOAD_SIZE);

  // Each new command should output the controller's previous motor mappings
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(PAYLOAD_MAP_NO_MOTORS, first_output, PAYLOAD_SIZE, "Motors should be unmapped before first command");
  TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(PAYLOAD_MAP_BOTH_MOTORS, second_output, PAYLOAD_SIZE, "Motors should both be mapped before second command");
}