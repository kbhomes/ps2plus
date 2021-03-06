#ifdef PS2PLUS_BOOTLOADER

#include "main.h"

#if 0
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#define DEBUG_PUTS(...) puts(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#define DEBUG_PUTS(...)
#endif

static unsigned long millis_init;

// Tracking whether a firmware update was initiated before the bootloader has 
// passed execution to the firmware.
static bool is_updating;

// Tracking whether the bootloader is waiting indefinitely for a firmware update.
static bool is_waiting;

// Indicates whether the special button combination to keep the bootloader 
// waiting indefinitely has been held since startup.
static bool is_wait_combo_pressed;

static void print_hex_array(const uint8_t *array, size_t length) {
  for (size_t i = 0; i < length; i++) {
    DEBUG_PRINTF("%02X ", array[i]);
  }
}

static void print_checksum_calculation(ps2plus_bootloader_update_record *record) {
  uint8_t checksum = 0;
  
  // Compute the checksum on all relevant components of the record
  DEBUG_PRINTF("[cs] %02X + %02X -> %02X\n", checksum, (uint8_t)(record->target_address & 0xFF), (uint8_t)(checksum + (uint8_t)(record->target_address & 0xFF)));
  checksum += record->target_address & 0xFF;
  
  DEBUG_PRINTF("[cs] %02X + %02X -> %02X\n", checksum, (uint8_t)((record->target_address >> 8) & 0xFF), (uint8_t)(checksum + (uint8_t)((record->target_address >> 8) & 0xFF)));
  checksum += (record->target_address >> 8) & 0xFF;
  
  DEBUG_PRINTF("[cs] %02X + %02X -> %02X\n", checksum, (uint8_t)((record->target_address >> 16) & 0xFF), (uint8_t)(checksum + (uint8_t)((record->target_address >> 16) & 0xFF)));
  checksum += (record->target_address >> 16) & 0xFF;
  
  DEBUG_PRINTF("[cs] %02X + %02X -> %02X\n", checksum, (uint8_t)((record->target_address >> 24) & 0xFF), (uint8_t)(checksum + (uint8_t)((record->target_address >> 24) & 0xFF)));
  checksum += (record->target_address >> 24) & 0xFF;
  
  DEBUG_PRINTF("[cs] %02X + %02X -> %02X\n", checksum, record->data_length, (uint8_t)(checksum + record->data_length));
  checksum += record->data_length;
  
  for (int i = 0; i < record->data_length && i < sizeof(record->data); i++) {
    DEBUG_PRINTF("[cs] %02X + %02X -> %02X\n", checksum, record->data[i], (uint8_t)(checksum + record->data[i]));
    checksum += record->data[i];
  }
}

bool check_wait_combo() {
  return (
    platform_controller_read_digital_button(DBL1) &&
    platform_controller_read_digital_button(DBL2) &&
    platform_controller_read_digital_button(DBR1) &&
    platform_controller_read_digital_button(DBR2)
  );
}

void handle_update_start_record(volatile controller_state *state) {
  DEBUG_PUTS("[bootloader] Erasing firmware");
  
  // At the start, erase the device's firmware
  if (platform_bootloader_erase_firmware()) {
    state->bootloader.status = BLStatusOk;
  } else {
    state->bootloader.error = BEEraseFailed;
    state->bootloader.status = BLStatusError;
  }
}

void handle_update_data_record(volatile controller_state *state) {
  DEBUG_PUTS("[bootloader] Handling update");
  
  volatile ps2plus_bootloader_update_record *record = &state->bootloader.update.record;
  
  bool ok = true;
  bool checksum_validity = ps2plus_bootloader_update_record_validate_checksum(record);
  platform_bootloader_update_record_address_validity address_validity = platform_bootloader_validate_update_record_address(record);

  // Check the checksum
  if (ok && !checksum_validity) {
    state->bootloader.error = BEInvalidChecksum;
    state->bootloader.status = BLStatusError;
    ok = false;
    
    DEBUG_PUTS("[bootloader] Checksum calculation:");
    print_checksum_calculation(record);
    DEBUG_PUTS("[bootloader] Calculated checksum did not match:");
    DEBUG_PRINTF("  Expected checksum: 0x%X\n", record->data_checksum);
    DEBUG_PRINTF("  Calculated checksum: 0x%X\n", ps2plus_bootloader_update_record_calculate_checksum(record));
    DEBUG_PRINTF("  Target address: 0x%lX\n", record->target_address);
    DEBUG_PRINTF("  Data length: 0x%X\n", record->data_length);
    DEBUG_PRINTF("  Data: "); print_hex_array(record->data, record->data_length); DEBUG_PUTS("");
  } 

  // Check the target address
  if (ok && address_validity == BLAddressInvalid) {
    state->bootloader.error = BEInvalidAddress;
    state->bootloader.status = BLStatusError;
    ok = false;
  } 

  // Perform the flashing
  if (ok && address_validity == BLAddressValid && !platform_bootloader_flash_update_record(record)) {
    state->bootloader.error = BEUnspecifiedError;
    state->bootloader.status = BLStatusError;
    ok = false;
  }

  if (ok) { 
    state->bootloader.status = BLStatusOk;
  }
}

void handle_update_end_record(volatile controller_state *state) {
  DEBUG_PUTS("[bootloader] Rebooting device");
  
  // Reboot the controller now that the firmware is updated
  platform_reset();
}

void read_controller_input(volatile controller_state *state) {
  // Read digital buttons
  for (ps2plus_controller_digital_button button = 0; button < NUM_DIGITAL_BUTTONS; button++) {
    digital_button_update(&state->input.digital_buttons[button], platform_controller_read_digital_button(button));
  }
}

void update_controller(volatile controller_state *state) {
  // Recompute controller state
  read_controller_input(state);
  controller_input_recompute(&state->input);
  controller_state_update_mode(state);
}

void main_init(volatile controller_state *state) {
  puts("[bootloader] Waiting 1 second for firmware update signal");
  millis_init = platform_timing_millis();
  is_updating = false;
  is_waiting = false;
  is_wait_combo_pressed = check_wait_combo();
}

void main_loop(volatile controller_state *state) {
  if (!is_waiting && !is_updating) {
    // Check if the user is still pressing the wait combo
    is_wait_combo_pressed = is_wait_combo_pressed && check_wait_combo();

    // Check if an update has been signaled
    if (state->bootloader.update.ready) {
      // Bootloader received an update signal, so remain here
      puts("[bootloader] Received update signal, remaining in bootloader");
      is_updating = true;
    }

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
  }

  update_controller(state);
  
  if (!state->bootloader.update.ready) {
    // Nothing required until this update is ready
    return;
  }
  
  switch (state->bootloader.update.record.type) {
    case BLRecordTypeStart:
      handle_update_start_record(state);
      break;
      
    case BLRecordTypeData:
      handle_update_data_record(state);
      break;
      
    case BLRecordTypeEnd:
      handle_update_end_record(state);
      break;
  }
  
  // Mark this update as processed
  state->bootloader.update.ready = false;
}

#endif