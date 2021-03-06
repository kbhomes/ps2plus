#ifndef TEST_COMMAND_HELPER_H
#define TEST_COMMAND_HELPER_H

#include <commands/command.h>
#include <platforms/platform.h>
#include <stdio.h>

extern controller_state state;

/**
 * @brief Reset the command output buffer (useful if testing multiple commands in sequence)
 */
void helper_reset_output();

/**
 * @brief Enable verbose debug printing to aid troubleshooting test failures
 */
void helper_enable_debug();

/**
 * @brief Initialize/reset all command helpers
 */
void helper_initialize();

/**
 * @brief Given a controller state and a specific command with payload, finds the appropriate command processor,
 *        executes it, and returns the data output
 * 
 * @param state Current state of the controller; may be modified by the command processor
 * @param command_id ID of the command (e.g.: 42h for the main polling command)
 * @param command_bytes Byte array of command payload, not including the 3-byte header
 * @param command_length Length of both the command payload and the processed data output
 * @return uint8_t* Byte array of length `command_length` as written by the command processor
 * }
 */
uint8_t *helper_run_command(uint8_t command_id, const uint8_t *command_bytes, size_t command_length);

void helper_print_hex_array(const uint8_t *array, size_t length); 
void helper_print_binary_byte(uint8_t value);

#endif /* TEST_COMMAND_HELPER_H */