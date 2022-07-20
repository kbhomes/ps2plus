#ifndef CONTROLLER_STATE_H
#define CONTROLLER_STATE_H

#include <shared/primitive_data.h>
#include "bootloader.h"
#include "custom_config.h"
#include "input.h"

typedef enum controller_analog_mode {
  CMDigital,    // digital buttons only (2 bytes of data)
  CMAnalog,     // digital buttons, analog joysticks (6 bytes of data)
  CMAnalogFull, // digital buttons, analog joysticks, button pressures (18 bytes of data)
} controller_analog_mode;

typedef struct {
  uint8_t mapping;
  uint8_t value;
} controller_rumble_motor;

typedef struct {
  struct {
#if defined(PS2PLUS_FIRMWARE)
    primitive_data /* uint64_t */ firmware;
    primitive_data /* array(char[32]) */ microcontroller;
    primitive_data /* uint16_t */ configuration;
#elif defined(PS2PLUS_BOOTLOADER)
    primitive_data /* uint64_t */ bootloader;
#endif
  } versions;

  /**
   * @brief Mode identifier sent in all SPI communication
   */
  uint8_t mode;
  
  /**
   * @brief Time (milliseconds) of the last communication packet from the console
   * 
   * Used as a "watchdog timer" to reset the controller state if the console
   * hasn't communicated recently enough. A value of UINT64_MAX indicates that
   * the state has already been reset.
   */
  uint64_t last_communication_time;
  
  /**
   * @brief Current button and joystick input
   */
  controller_input input;
  
#ifdef PS2PLUS_FIRMWARE

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

  /**
   * @brief Rumble motor information for the small motor (FFh = on)
   */
  controller_rumble_motor rumble_motor_small;

  /**
   * @brief Rumble motor information for the large motor (40h+ = on)
   */
  controller_rumble_motor rumble_motor_large;

  /**
   * @brief Custom configuration information, persisted in internal storage
   */
  controller_custom_config custom_config;
  
  /**
   * @brief Time (milliseconds) of when the configuration reset combo was first held down.
   * 
   * A value of UINT64_MAX means the button combo is not currently being held down.
   * This is set to the current time when the user first presses the button combo down.
   * After a certain time threshold, the combo is considered fulfilled and the user's
   * custom configuration is reset.
   */
  uint64_t last_configuration_reset_combo_time;
#endif
  
#ifdef PS2PLUS_BOOTLOADER
  /**
   * @brief State information required for the operation of the bootloader
   */
  controller_bootloader bootloader;
#endif
} controller_state;

void controller_state_initialize(controller_state *);
void controller_state_update_mode(controller_state *);
void controller_state_set_versions(controller_state *, uint64_t firmware, const char microcontroller[32], uint16_t configuration, uint64_t bootloader);
void controller_state_reset(controller_state *);

#endif /* CONTROLLER_STATE_H */