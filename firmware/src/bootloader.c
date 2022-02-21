#ifdef PS2PLUS_BOOTLOADER

#include "main.h"

void main_init(volatile controller_state *state) {
  // TODO: Setup interrupts
}

void main_loop(volatile controller_state *state) {
  if (!state->bootloader.update.ready) {
    return;
  }
  
  if (state->bootloader.update.record_type == BLRecordTypeStart) {
    state->bootloader.status = BLStatusOk;
  } else if (state->bootloader.update.record_type == BLRecordTypeEnd) {
    // TODO: Reboot the controller
  } else if (state->bootloader.update.record_type == BLRecordTypeData) {
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