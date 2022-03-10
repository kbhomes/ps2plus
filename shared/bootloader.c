#include "bootloader.h"

uint8_t ps2plus_bootloader_update_record_calculate_checksum(ps2plus_bootloader_update_record *record) {
  uint8_t checksum = 0;
  
  // Compute the checksum on all relevant components of the record
  checksum += record->target_address & 0xFF;
  checksum += (record->target_address >> 8) & 0xFF;
  checksum += (record->target_address >> 16) & 0xFF;
  checksum += (record->target_address >> 24) & 0xFF;
  checksum += record->data_length;
  
  for (int i = 0; i < sizeof(record->data); i++) {
    checksum += record->data[i];
  }
  
  return checksum;
}

bool ps2plus_bootloader_update_record_validate_checksum(ps2plus_bootloader_update_record *record) {
  return record->data_checksum == ps2plus_bootloader_update_record_calculate_checksum(record);
}