#ifndef TEST_POLLING_HELPER_H
#define TEST_POLLING_HELPER_H

#include <commands/command.h>
#include <platforms/platform.h>
#include <stdio.h>

extern controller_state state;

// Total sizes of the command payloads for the different controller modes
#define POLLING_PAYLOAD_CONFIG_SIZE 6
#define POLLING_PAYLOAD_DIGITAL_SIZE 2
#define POLLING_PAYLOAD_ANALOG_SIZE 6
#define POLLING_PAYLOAD_ANALOG_FULL_SIZE 18

/**
 * @brief Returns the number of bytes expected in the command payload for the given controller mode 
 */
size_t polling_payload_size_for_mode(bool config_mode, controller_analog_mode analog_mode);

/**
 * @brief Tests the given polling command (i.e.: 42h or 43h) to ensure the digital button output is correct
 */
void polling_check_digital_buttons(uint8_t command_id, const uint8_t *payload);

/**
 * @brief Tests the given polling command (i.e.: 42h or 43h) to ensure the analog joystick output is correct
 */
void polling_check_analog_joysticks(uint8_t command_id, const uint8_t *payload);

/**
 * @brief Tests the given polling command (i.e.: 42h or 43h) to ensure the button pressure output is correct
 */
void polling_check_analog_pressures(uint8_t command_id, const uint8_t *payload);

#endif /* TEST_POLLING_HELPER_H */