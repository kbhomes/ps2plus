#ifdef PLATFORM_PIC16F

#include <platforms/platform.h>

// Using the XC8 compiler like so worked?
// xc8-cc -mcpu=16F877A -Ilib/PS2Plus -DPLATFORM_PIC16F ./src/main.c ./lib/PS2Plus/commands/button_inclusions_command.c ./lib/PS2Plus/commands/command.c ./lib/PS2Plus/commands/configur e_analog_response_command.c ./lib/PS2Plus/commands/device_descriptor_46_command.c ./lib/PS2Plus/commands/device_descriptor_47_command.c ./lib/PS2Plus/commands/device_descriptor_4c_com mand.c ./lib/PS2Plus/commands/initialize_pressure_sensor_command.c ./lib/PS2Plus/commands/main_polling_command.c ./lib/PS2Plus/commands/map_motors_command.c ./lib/PS2Plus/commands/sta tus_info_command.c ./lib/PS2Plus/commands/switch_digital_analog_mode_command.c ./lib/PS2Plus/controller_input.c ./lib/PS2Plus/controller_state.c ./lib/PS2Plus/platforms/arduino.c ./li b/PS2Plus/platforms/native.c ./lib/PS2Plus/platforms/pic16f.c ./lib/PS2Plus/utilities/debounced_bool.c ./lib/PS2Plus/utilities/latching_bool.c

void platform_init() {
  
}

unsigned long platform_timing_millis() {
  return 0UL;
}

void platform_timing_sleep(unsigned int millis) {
  
}

uint8_t platform_spi_read() {
  return 0;
}

void platform_spi_write(uint8_t value) {
  
}

#endif