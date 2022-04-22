#include "main.h"

#include <math.h>

volatile controller_state state;
volatile command_packet packet;

void write_with_ack(uint8_t value) {
  platform_spi_playstation_ack();
  platform_spi_playstation_write(value);
}

void interrupt_handler() {
#if defined(PS2PLUS_FIRMWARE)
  command_packet_step(&packet, &state, platform_spi_playstation_read());
#elif defined(PS2PLUS_BOOTLOADER)
  // DEV: Nothing! Trying to determine whether the right interrupt executes in firmware mode.
#endif
}

inline float clampf(float min, float max, float value) {
  return fmaxf(min, fminf(max, value));
}

void apply_custom_configuration() {
  if (state.analog_mode == CMAnalog || state.analog_mode == CMAnalogFull) {
    if (state.custom_config.values.enable_joystick_axis_range_remapping.boolean) {
      for (ps2plus_controller_joystick_axis a = 0; a < NUM_JOYSTICK_AXES; a++) {
        uint8_t axis_value = state.input.joysticks[a];
        uint8_t remap_min = state.custom_config.values.joystick_axis_range_remapping[a*3 + 0].uint8;
        uint8_t remap_ctr = state.custom_config.values.joystick_axis_range_remapping[a*3 + 1].uint8;
        uint8_t remap_max = state.custom_config.values.joystick_axis_range_remapping[a*3 + 2].uint8;
        
        if (axis_value < remap_ctr) {
          state.input.joysticks[a] = (uint8_t)(0x7F * clampf(0.f, 1.f, (float)(axis_value - remap_min)/(remap_ctr - remap_min)));
        } else {
          state.input.joysticks[a] = 0x80 + (uint8_t)(0x7F * clampf(0.f, 1.f, (float)(axis_value - remap_ctr)/(remap_max - remap_ctr)));
        }
      }
    }
    
    uint8_t deadzone_left = state.custom_config.values.joystick_deadzone_left.uint8;
    uint8_t deadzone_right = state.custom_config.values.joystick_deadzone_right.uint8;
    
    if (deadzone_left) {
      uint8_t js_left_x = state.input.joysticks[JSLeftX];
      uint8_t js_left_y = state.input.joysticks[JSLeftY];
      
      if ((js_left_x > 0x7F - deadzone_left) && (js_left_x < 0x80 + deadzone_left)) {
        state.input.joysticks[JSLeftX] = 0x7F;
      }
      
      if ((js_left_y > 0x7F - deadzone_left) && (js_left_y < 0x80 + deadzone_left)) {
        state.input.joysticks[JSLeftY] = 0x7F;
      }
    }
    
    if (deadzone_right) {
      uint8_t js_right_x = state.input.joysticks[JSRightX];
      uint8_t js_right_y = state.input.joysticks[JSRightY];
      
      if ((js_right_x > 0x7F - deadzone_right) && (js_right_x < 0x80 + deadzone_right)) {
        state.input.joysticks[JSRightX] = 0x7F;
      }
      
      if ((js_right_y > 0x7F - deadzone_right) && (js_right_y < 0x80 + deadzone_right)) {
        state.input.joysticks[JSRightY] = 0x7F;
      }
    }
  }
  
  if (state.analog_mode == CMDigital) {
    if (state.custom_config.values.joystick_digital_enable_left.boolean) {
      uint8_t js_left_x = state.input.joysticks[JSLeftX];
      uint8_t js_left_y = state.input.joysticks[JSLeftY];
      uint8_t threshold = state.custom_config.values.joystick_digital_threshold_left.uint8;
      debounced_bool *dd_left = &state.input.digital_buttons[DDLeft];
      debounced_bool *dd_right = &state.input.digital_buttons[DDRight];
      debounced_bool *dd_up = &state.input.digital_buttons[DDUp];
      debounced_bool *dd_down = &state.input.digital_buttons[DDDown];

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
    
    if (state.custom_config.values.joystick_digital_enable_right.boolean) {
      uint8_t js_right_x = state.input.joysticks[JSRightX];
      uint8_t js_right_y = state.input.joysticks[JSRightY];
      uint8_t threshold = state.custom_config.values.joystick_digital_threshold_right.uint8;
      debounced_bool *db_square = &state.input.digital_buttons[DBSquare];
      debounced_bool *db_circle = &state.input.digital_buttons[DBCircle];
      debounced_bool *db_triangle = &state.input.digital_buttons[DBTriangle];
      debounced_bool *db_cross = &state.input.digital_buttons[DBCross];

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

void update_controller() {
  // Read digital button values
  for (ps2plus_controller_digital_button button = 0; button < NUM_DIGITAL_BUTTONS; button++) {
    bool active_low_button_state = !platform_controller_read_digital_button(button);
    debounced_force(&state.input.digital_buttons[button], active_low_button_state);
    // debounced_update(&state.input.digital_buttons[button], active_low_button_state);
  }

  // Read joystick values
  for (ps2plus_controller_joystick_axis joystick = 0; joystick < NUM_JOYSTICK_AXES; joystick++) {
    state.input.joysticks[joystick] = platform_controller_read_joystick(joystick);
  }
  
  // Apply custom configuration
  if (state.custom_config.enabled) {
    apply_custom_configuration();
  }
  
  // Recompute controller state
  controller_input_recompute(&state.input);
  controller_state_update_mode(&state);
  
  // Update the analog mode LED
  platform_controller_set_analog_led(state.analog_mode == CMAnalog || state.analog_mode == CMAnalogFull);
}

void handle_transmission() {
  if (!platform_spi_playstation_selected()) {
    // Prepare for the next packet
    platform_spi_playstation_write(0xFF);
    command_packet_initialize(&packet, &write_with_ack);
  }
}

int main(void) {
  platform_init(&interrupt_handler);
  
  // Set the initial internal state of the firmware
  controller_state_initialize(&state);
  controller_state_set_versions(&state, VERSION_FIRMWARE, VERSION_MICROCONTROLLER, VERSION_CONFIGURATION, VERSION_BOOTLOADER);
  command_packet_initialize(&packet, &write_with_ack);
  
  // Execute the initialization for the bootloader/firmware
  main_init(&state);

  while (true) {
    update_controller();
    handle_transmission();
    main_loop(&state);
  }

  return 0;
}