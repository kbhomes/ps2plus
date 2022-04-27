#ifdef PS2PLUS_FIRMWARE

#include "main.h"

#include <math.h>

void check_configuration_version(volatile controller_state *state) {
  // If the stored configuration version does not match this firmware's version, reset
  // all custom configuration and save it back to storage
  if (state->custom_config.configuration_version != VERSION_CONFIGURATION) {
    puts("[firmware] Resetting config due to mismatching versions");
    controller_custom_config_initialize(&state->custom_config);
    controller_custom_config_save(&state->custom_config);
  }
}

void debug_versions_dump(volatile controller_state *state) {
  printf("[version] Firmware = %u\n", VERSION_FIRMWARE);
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
      debounced_bool *dd_left = &state->input.digital_buttons[DDLeft];
      debounced_bool *dd_right = &state->input.digital_buttons[DDRight];
      debounced_bool *dd_up = &state->input.digital_buttons[DDUp];
      debounced_bool *dd_down = &state->input.digital_buttons[DDDown];

      if (debounced_read(dd_left) && js_left_x < 0x7F - threshold) {
        debounced_force(dd_left, false);
      }
      
      if (debounced_read(dd_right) && js_left_x > 0x80 + threshold) {
        debounced_force(dd_right, false);
      }
      
      if (debounced_read(dd_up) && js_left_y < 0x7F - threshold) {
        debounced_force(dd_up, false);
      }
      
      if (debounced_read(dd_down) && js_left_y > 0x80 + threshold) {
        debounced_force(dd_down, false);
      }
    }
    
    if (state->custom_config.values.joystick_digital_enable_right.boolean) {
      uint8_t js_right_x = state->input.joysticks[JSRightX];
      uint8_t js_right_y = state->input.joysticks[JSRightY];
      uint8_t threshold = state->custom_config.values.joystick_digital_threshold_right.uint8;
      debounced_bool *db_square = &state->input.digital_buttons[DBSquare];
      debounced_bool *db_circle = &state->input.digital_buttons[DBCircle];
      debounced_bool *db_triangle = &state->input.digital_buttons[DBTriangle];
      debounced_bool *db_cross = &state->input.digital_buttons[DBCross];

      if (debounced_read(db_square) && js_right_x < 0x7F - threshold) {
        debounced_force(db_square, false);
      }
      
      if (debounced_read(db_circle) && js_right_x > 0x80 + threshold) {
        debounced_force(db_circle, false);
      }
      
      if (debounced_read(db_triangle) && js_right_y < 0x7F - threshold) {
        debounced_force(db_triangle, false);
      }
      
      if (debounced_read(db_cross) && js_right_y > 0x80 + threshold) {
        debounced_force(db_cross, false);
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
    
    bool active_low_button_state = !platform_controller_read_digital_button(source);
    debounced_force(&state->input.digital_buttons[button], active_low_button_state);
    // debounced_update(&state->input.digital_buttons[button], active_low_button_state);
  }

  // Read joystick values
  for (ps2plus_controller_joystick_axis joystick = 0; joystick < NUM_JOYSTICK_AXES; joystick++) {
    state->input.joysticks[joystick] = platform_controller_read_joystick(joystick);
  }
  
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
  
  // Update the analog mode LED
  platform_controller_set_analog_led(state->analog_mode == CMAnalog || state->analog_mode == CMAnalogFull);
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
}

#endif