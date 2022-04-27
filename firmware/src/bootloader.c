#ifdef PS2PLUS_BOOTLOADER

#include "main.h"

static unsigned long millis_init;

// Tracking whether a firmware update was initiated before the bootloader has 
// passed execution to the firmware.
static bool is_updating;

// Tracking whether the bootloader is waiting indefinitely for a firmware update.
static bool is_waiting;

// Indicates whether the special button combination to keep the bootloader 
// waiting indefinitely has been held since startup.
static bool is_wait_combo_pressed;

bool check_wait_combo() {
  return (
    platform_controller_read_digital_button(DBL1) &&
    platform_controller_read_digital_button(DBL2) &&
    platform_controller_read_digital_button(DBR1) &&
    platform_controller_read_digital_button(DBR2)
  );
}

void main_init(volatile controller_state *state) {
  puts("[bootloader] Waiting 1 second for firmware update signal");
  millis_init = platform_timing_millis();
  is_updating = false;
  is_wait_combo_pressed = check_wait_combo();
}

void main_loop(volatile controller_state *state) {
  if (!is_waiting && !is_updating) {
    // Check if the user is still pressing the wait combo
    is_wait_combo_pressed = is_wait_combo_pressed && check_wait_combo();

    // Check if the one-second wait has expired
    if (platform_timing_millis() - millis_init > 1000ul) {
      if (is_wait_combo_pressed) {
        // If the wait combo has been held since startup, set the wait flag
        puts("[bootloader] Wait button combo detected, remaining in bootloader");
        is_waiting = true;
      } else {
        // Transfer execution to the firmware
        puts("[bootloader] Didn't receive update signal in 1 second, transferring execution to firmware");
        platform_bootloader_execute_firmware();
      }
    }

    // Check if an update has been signaled
    if (state->bootloader.update.ready) {
      // Bootloader received an update signal, so remain here
      puts("[bootloader] Received update signal, remaining in bootloader");
      is_updating = true;
    }
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