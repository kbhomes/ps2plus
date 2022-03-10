#ifdef PS2PLUS_BOOTLOADER

#include "main.h"

void main_init(volatile controller_state *state) {
  puts("[bootloader] Initializing");
  puts("[bootloader] Waiting one second for firmware update signal");
  platform_timing_sleep(1000);
  
  if (platform_controller_read_digital_button(DBTriangle)) {
    puts("[bootloader] Remaining in bootloader");
  } else {
    // Transfer execution to the firmware
    puts("[bootloader] Transferring execution to firmware");
    platform_bootloader_execute_firmware();
  }
  
//  // DEV: Generate 5 seconds of LED pulses to indicate the bootloader is executing
//  for (int i = 0; i < 5; i++) {
//    platform_timing_sleep(500);
//    platform_controller_set_analog_led(true);
//    platform_timing_sleep(500);
//    platform_controller_set_analog_led(false);
//  }
//  
//  // TODO: If a bootloader signal wasnt received within a set time,
//  // transfer execution to the firmware. Otherwise, remain within the
//  // bootloader.
}

void main_loop(volatile controller_state *state) {
//  puts("[bootloader] Loop...");

//  if (!state->bootloader.update.ready) {
//    return;
//  }
//  
//  if (state->bootloader.update.record_type == BLRecordTypeStart) {
//    state->bootloader.status = BLStatusOk;
//  } else if (state->bootloader.update.record_type == BLRecordTypeEnd) {
//    // TODO: Reboot the controller
//  } else if (state->bootloader.update.record_type == BLRecordTypeData) {
//    // TODO: Check data validity with checksum
//    // TODO: Check address validity with bootloader ranges
//    // TODO: Check first instruction validity with expected jump instruction
//    // TODO: Write data to program memory
//    state->bootloader.status = BLStatusOk;
//  }
//  
//  // Mark this update as processed
//  state->bootloader.update.ready = false;
}

#endif