#ifndef CONTROLLER_STATE_H
#define CONTROLLER_STATE_H

#include "controller_input.h"

enum controller_analog_mode {
  CMDigital    = 0x41, // digital buttons only (2 bytes of data)
  CMAnalog     = 0x73, // digital buttons, analog joysticks (6 bytes of data)
  CMAnalogFull = 0x79, // digital buttons, analog joysticks, button pressures (18 bytes of data)
};

typedef struct {
  uint8_t mapping;
  uint8_t value;
} controller_rumble_motor;

typedef struct {
  /**
   * @brief Current button and joystick input
   */
  controller_input input;

  /**
   * @brief Current digital/analog mode of the controller
   */
  controller_analog_mode analog_mode;

  /**
   * @brief If true, the analog mode of the controller has been locked by 
   *        the console and cannot be switched by the user
   */
  bool analog_mode_locked;

  /**
   * @brief Whether the controller has entered configuration mode (F3h)
   */
  bool config_mode;

  controller_rumble_motor rumble_motor_small;
  controller_rumble_motor rumble_motor_large;
} controller_state;

void controller_state_initialize(controller_state *);
uint8_t controller_state_get_mode(controller_state *);
bool controller_state_is_digital(controller_state *);
bool controller_state_is_analog(controller_state *);
bool controller_state_include_button_pressure(controller_state *);

#endif /* CONTROLLER_STATE_H */