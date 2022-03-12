#ifdef PS2PLUS_BOOTLOADER

#include "main.h"

#include <xc.h>

void main_init(volatile controller_state *state) {
  GIE = 0;
  
  puts("[bootloader] Initializing");
  printf("[bootloader] Erasing firmware: %02X\n", platform_bootloader_calculate_firmware_checksum());
  
  // Infinite loop!
  while (true) { }
  
//  puts("[bootloader] Waiting one second for firmware update signal");
//  platform_timing_sleep(1000);
//  
//  if (platform_controller_read_digital_button(DBTriangle)) {
//    puts("[bootloader] Remaining in bootloader");
//  } else {
//    // Transfer execution to the firmware
//    puts("[bootloader] Transferring execution to firmware");
//    platform_bootloader_execute_firmware();
//  }
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