#ifdef PS2PLUS_FIRMWARE

#include "main.h"

#include <math.h>
#include <inttypes.h>

#define CONTROLLER_WATCHDOG_MILLIS 1000
#define CONTROLLER_CONFIGURATION_RESET_MILLIS 5000

void check_configuration_version(volatile controller_state *state) {
  // If the stored configuration version does not match this firmware's version, reset
  // all custom configuration and save it back to storage
  if (state->custom_config.configuration_version != VERSION_CONFIGURATION) {
    puts("[firmware] Resetting config due to mismatching versions");
    controller_custom_config_erase(&state->custom_config);
  }
}

void debug_versions_dump(volatile controller_state *state) {
  printf("[version] Firmware = " PRIu64 "\n", VERSION_FIRMWARE);
  printf("[version] Microcontroller = %s\n", VERSION_MICROCONTROLLER);
  printf("[version] Configuration = %u\n", VERSION_CONFIGURATION);
}

inline float clampf(float min, float max, float value) {
  return fmaxf(min, fminf(max, value));
}

/**
 * After controller inputs are read, applies any enabled custom configuration
 * to modify controller behavior.
 */
void apply_custom_configuration(volatile controller_state *state) {
  // Note: button remapping occurs at the input-read stage
  
  if (state->analog_mode == CMAnalog || state->analog_mode == CMAnalogFull) {
    // Remap joystick ranges, if necessary
    if (state->custom_config.values.enable_joystick_axis_range_remapping.boolean) {
      for (ps2plus_controller_joystick_axis a = 0; a < NUM_JOYSTICK_AXES; a++) {
        uint8_t axis_value = state->input.joysticks[a];
        uint8_t remap_min = state->custom_config.values.joystick_axis_range_remapping[a*3 + 0].uint8;
        uint8_t remap_ctr = state->custom_config.values.joystick_axis_range_remapping[a*3 + 1].uint8;
        uint8_t remap_max = state->custom_config.values.joystick_axis_range_remapping[a*3 + 2].uint8;
        
        if (axis_value < remap_ctr) {
          state->input.joysticks[a] = (uint8_t)(0x7F * clampf(0.f, 1.f, (float)(axis_value - remap_min)/(remap_ctr - remap_min)));
        } else {
          state->input.joysticks[a] = 0x80 + (uint8_t)(0x7F * clampf(0.f, 1.f, (float)(axis_value - remap_ctr)/(remap_max - remap_ctr)));
        }
      }
    }
    
    // Apply joystick deadzones
    uint8_t deadzone_left = state->custom_config.values.joystick_deadzone_left.uint8;
    uint8_t deadzone_right = state->custom_config.values.joystick_deadzone_right.uint8;
    
    if (deadzone_left) {
      uint8_t js_left_x = state->input.joysticks[JSLeftX];
      uint8_t js_left_y = state->input.joysticks[JSLeftY];
      
      if ((js_left_x > 0x7F - deadzone_left) && (js_left_x < 0x80 + deadzone_left)) {
        state->input.joysticks[JSLeftX] = 0x7F;
      }
      
      if ((js_left_y > 0x7F - deadzone_left) && (js_left_y < 0x80 + deadzone_left)) {
        state->input.joysticks[JSLeftY] = 0x7F;
      }
    }
    
    if (deadzone_right) {
      uint8_t js_right_x = state->input.joysticks[JSRightX];
      uint8_t js_right_y = state->input.joysticks[JSRightY];
      
      if ((js_right_x > 0x7F - deadzone_right) && (js_right_x < 0x80 + deadzone_right)) {
        state->input.joysticks[JSRightX] = 0x7F;
      }
      
      if ((js_right_y > 0x7F - deadzone_right) && (js_right_y < 0x80 + deadzone_right)) {
        state->input.joysticks[JSRightY] = 0x7F;
      }
    }
  }
  
  if (state->analog_mode == CMDigital) {
    if (state->custom_config.values.joystick_digital_enable_left.boolean) {
      uint8_t js_left_x = state->input.joysticks[JSLeftX];
      uint8_t js_left_y = state->input.joysticks[JSLeftY];
      uint8_t threshold = state->custom_config.values.joystick_digital_threshold_left.uint8;
      digital_button *dd_left = &state->input.digital_buttons[DDLeft];
      digital_button *dd_right = &state->input.digital_buttons[DDRight];
      digital_button *dd_up = &state->input.digital_buttons[DDUp];
      digital_button *dd_down = &state->input.digital_buttons[DDDown];

      if (!digital_button_is_down(dd_left) && js_left_x < 0x7F - threshold) {
        digital_button_update(dd_left, true);
      }
      
      if (!digital_button_is_down(dd_right) && js_left_x > 0x80 + threshold) {
        digital_button_update(dd_right, true);
      }
      
      if (!digital_button_is_down(dd_up) && js_left_y < 0x7F - threshold) {
        digital_button_update(dd_up, true);
      }
      
      if (!digital_button_is_down(dd_down) && js_left_y > 0x80 + threshold) {
        digital_button_update(dd_down, true);
      }
    }
    
    if (state->custom_config.values.joystick_digital_enable_right.boolean) {
      uint8_t js_right_x = state->input.joysticks[JSRightX];
      uint8_t js_right_y = state->input.joysticks[JSRightY];
      uint8_t threshold = state->custom_config.values.joystick_digital_threshold_right.uint8;
      digital_button *db_square = &state->input.digital_buttons[DBSquare];
      digital_button *db_circle = &state->input.digital_buttons[DBCircle];
      digital_button *db_triangle = &state->input.digital_buttons[DBTriangle];
      digital_button *db_cross = &state->input.digital_buttons[DBCross];

      if (!digital_button_is_down(db_square) && js_right_x < 0x7F - threshold) {
        digital_button_update(db_square, true);
      }
      
      if (!digital_button_is_down(db_circle) && js_right_x > 0x80 + threshold) {
        digital_button_update(db_circle, true);
      }
      
      if (!digital_button_is_down(db_triangle) && js_right_y < 0x7F - threshold) {
        digital_button_update(db_triangle, true);
      }
      
      if (!digital_button_is_down(db_cross) && js_right_y > 0x80 + threshold) {
        digital_button_update(db_cross, true);
      }
    }
  }
}

void read_controller_input(volatile controller_state *state) {
  // Read digital buttons
  for (ps2plus_controller_digital_button button = 0; button < NUM_DIGITAL_BUTTONS; button++) {
    ps2plus_controller_digital_button source = button;
    
    // Apply button remapping, if enabled
    if (state->custom_config.values.enable_button_remapping.boolean) {
      source = (ps2plus_controller_digital_button)state->custom_config.values.button_remapping[button].uint8;
    }
    
     digital_button_update(&state->input.digital_buttons[button], platform_controller_read_digital_button(source));
  }

  // Read joystick values
  for (ps2plus_controller_joystick_axis joystick = 0; joystick < NUM_JOYSTICK_AXES; joystick++) {
    state->input.joysticks[joystick] = platform_controller_read_joystick(joystick);
  }
  
  // Update the analog button
  digital_button_update(&state->input.analog_button, platform_controller_read_analog_button());
  
  // Apply custom configuration
  if (state->custom_config.enabled) {
    apply_custom_configuration(state);
  }
}

void update_controller(volatile controller_state *state) {
  // Recompute controller state
  read_controller_input(state);
  controller_input_recompute(&state->input);
  controller_state_update_mode(state);
  
  // If analog mode isn't locked, allow switching between digital and analog
  if (!state->analog_mode_locked && digital_button_is_pressed(&state->input.analog_button)) {
    state->analog_mode = (state->analog_mode == CMDigital) ? CMAnalog : CMDigital;
  }
  
  // Update the analog mode LED -- but only if the configuration reset button combo isn't attempting to drive it
  // TODO: Replace this check with a state field, in case we want to make the analog LED a more generic information
  //       communication mechanism (maybe `bool is_led_controlled`) used by systems other than the analog indicator
  //       and the configuration reset button combo.
  if (state->last_configuration_reset_combo_time == UINT64_MAX) {
    platform_controller_set_analog_led(state->analog_mode == CMAnalog || state->analog_mode == CMAnalogFull);
  }
  
  // Update the rumble motors
  platform_controller_set_motor_small(state->rumble_motor_small.mapping == 0x00 && state->rumble_motor_small.value == 0xFF);
  platform_controller_set_motor_large(state->rumble_motor_large.mapping == 0x01 ? state->rumble_motor_large.value : 0x00);
}

void check_watchdog_timer(volatile controller_state *state) {
  if (state->last_communication_time == UINT64_MAX) {
    return;
  }
  
  uint64_t now = platform_timing_millis();
  uint64_t delta = now - state->last_communication_time;
  
  if (delta > CONTROLLER_WATCHDOG_MILLIS) {
    puts("[firmware] Watchdog: resetting controller state");
    controller_state_reset(state);
  }
}

void check_configuration_reset_combo(volatile controller_state *state) {
  bool reset_timer_active = state->last_configuration_reset_combo_time != UINT64_MAX;
  bool reset_combo_down = (
    platform_controller_read_digital_button(DBTriangle) &&
    platform_controller_read_digital_button(DBL1) &&
    platform_controller_read_digital_button(DBL2) &&
    platform_controller_read_digital_button(DBR1) &&
    platform_controller_read_digital_button(DBR2)
  );
  
  if (reset_combo_down) {
    if (reset_timer_active) {
      uint64_t delta = platform_timing_millis() - state->last_configuration_reset_combo_time;
      
      // If enough time has elapsed that the combo is fulfilled, then flash the LED to signal
      // to the user that the time threshold has been satisfied
      if (delta >= CONTROLLER_CONFIGURATION_RESET_MILLIS) {
        // Cycle the LED every 250ms
        platform_controller_set_analog_led(delta % 250 >= 125);
      }
    } else {
      // Combo is newly held down so begin the timer
      puts("[firmware] Configuration reset button combo identified");
      state->last_configuration_reset_combo_time = platform_timing_millis();
    }
  } else if (reset_timer_active) {
    uint64_t delta = platform_timing_millis() - state->last_configuration_reset_combo_time;

    // If the threshold has elapsed by the time the user lets go of the combo,
    // consider the combo fulfilled and reset the configuration
    if (delta >= CONTROLLER_CONFIGURATION_RESET_MILLIS) {
      puts("[firmware] Resetting config due to button combo");
      controller_custom_config_erase(&state->custom_config);
    }
    
    // Reset the timer
    state->last_configuration_reset_combo_time = UINT64_MAX;
  }
}

void main_init(volatile controller_state *state) {
  puts("[firmware] Initializing");
  debug_versions_dump(state);
  
  // Load the configuration from storage and check the configuration version
  controller_custom_config_load(&state->custom_config);
  check_configuration_version(state);
}

void main_loop(volatile controller_state *state) {
  update_controller(state);
  check_watchdog_timer(state);
  check_configuration_reset_combo(state);
}

#endif