#ifdef PS2PLUS_BOOTLOADER

#include "main.h"

static unsigned long millis_init;
static bool is_updating;

void main_init(volatile controller_state *state) {
  puts("[bootloader] Waiting 1 second for firmware update signal");
  millis_init = platform_timing_millis();
  is_updating = false;
}

void main_loop(volatile controller_state *state) {
  unsigned long millis_now = platform_timing_millis();

  printf("MS: %lu\n", millis_now);
  
  if (!is_updating && millis_now - millis_init > 1000ul) {
    // Transfer execution to the firmware
    puts("[bootloader] Didn't receive update signal in 1 second, transferring execution to firmware");
    platform_bootloader_execute_firmware();
  }
  
  if (!is_updating && state->bootloader.update.ready) {
    // Bootloader received an update signal, so remain here
    puts("[bootloader] Received update signal, remaining in bootloader");
    is_updating = true;
  }
  
  if (!state->bootloader.update.ready) {
    // Nothing required until this update is ready
    return;
  }
  
  if (state->bootloader.update.record.type == BLRecordTypeStart) {
    state->bootloader.status = BLStatusOk;
  } else if (state->bootloader.update.record.type == BLRecordTypeEnd) {
    // Reboot the controller now that the firmware is updated
    platform_reset();
  } else if (state->bootloader.update.record.type == BLRecordTypeData) {
    // TODO: Check data validity with checksum
    // TODO: Check address validity with bootloader ranges
    // TODO: Check first instruction validity with expected jump instruction
    // TODO: Write data to program memory
    state->bootloader.status = BLStatusOk;
  }
  
  // Mark this update as processed
  state->bootloader.update.ready = false;
}

#endif