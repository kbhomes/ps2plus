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
bool platform_controller_read_digital_button(controller_input_digital_button button);

/**
 * @brief Read the raw analog joystick value for the given joystick's axis
 * 
 * @param joystick_axis Left/right joystick's X/Y axis
 * @return uint8_t Analog value between 0x00 (-1.0) and 0xFF (1.0)
 */
uint8_t platform_controller_read_joystick(controller_input_joystick_axis joystick_axis);

/**
 * @brief Enable or disable the controller's analog LED
 * 
 * @param active Whether to enable or disable the LED
 */
void platform_controller_set_analog_led(bool active);

#endif /* PLATFORMS_CONTROLLER_H */