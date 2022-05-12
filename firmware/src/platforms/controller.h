#ifndef PLATFORMS_CONTROLLER_H
#define PLATFORMS_CONTROLLER_H

#include <controller/state.h>

/**
 * @brief Read the digital value of the given controller button
 * 
 * @param button ID of the button to read
 * @return true if the button is currently being pressed
 * @return false otherwise
 */
bool platform_controller_read_digital_button(ps2plus_controller_digital_button button);

/**
 * @brief Read the raw analog joystick value for the given joystick's axis
 * 
 * @param joystick_axis Left/right joystick's X/Y axis
 * @return uint8_t Analog value between 0x00 (-1.0) and 0xFF (1.0)
 */
uint8_t platform_controller_read_joystick(ps2plus_controller_joystick_axis joystick_axis);

/**
 * @brief Reads the digital value of the analog toggle button
 * @return true if the analog toggle button is currently being pressed
 * @return false otherwise
 */
bool platform_controller_read_analog_button();

/**
 * @brief Enable or disable the controller's analog LED
 * 
 * @param active Whether to enable or disable the LED
 */
void platform_controller_set_analog_led(bool active);

/**
 * @brief Enable or disable the controller's large motor
 * 
 * TODO: The large motor supports power levels between 0x00 and 0xFF, corresponding
 *       to the voltage range between 0V and MP (~7-8V, provided by the console).
 *       Current implementation of the large motor only supports outputting either
 *       off or maximum (if `power` >= 0x40). This method should eventually allow
 *       outputting on the full voltage range.
 */
void platform_controller_set_motor_large(uint8_t power);

/**
 * @brief Enable or disable the controller's small motor (either full power or off)
 */
void platform_controller_set_motor_small(bool active);

#endif /* PLATFORMS_CONTROLLER_H */