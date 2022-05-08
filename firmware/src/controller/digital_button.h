#ifndef CONTROLLER_DIGITAL_BUTTON_H
#define CONTROLLER_DIGITAL_BUTTON_H

#include <stdint.h>
#include <utilities/debounced_bool.h>

/**
 * Structure that tracks an individual digital button, allowing smarter access
 * to the button state.
 */
typedef struct {
  bool previous;
  bool down;
} digital_button;

/**
 * @brief Initializes the digital button (resets internal state) 
 */
void digital_button_init(digital_button *);

/**
 * @brief Updates the current state of the button. 
 * 
 * Should be called on every frame with the current raw value of the hardware
 * button (true if the button is currently being held down).
 */
void digital_button_update(digital_button *, bool down);

/**
 * @brief Returns true if the button is currently being held down.
 */
bool digital_button_is_down(digital_button *);

/**
 * @brief Returns true if the button was first pressed in the current frame.
 * 
 * Internally checks if the button was not held down in the previous frame
 * but is held down in the current frame (indicating a new button press). In
 * the next frame, this method will return false.
 */
bool digital_button_is_pressed(digital_button *);

/**
 * @brief Returns true if the button was released in the current frame.
 * 
 * Internally checks if the button was held down in the previous frame but is
 * not held down in the current frame (indicating the end of a button press).
 * In the next frame, this method will return false.
 */
bool digital_button_is_released(digital_button *);

#endif	/* CONTROLLER_DIGITAL_BUTTON_H */

