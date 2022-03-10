#ifndef SHARED_BOOTLOADER_H
#define	SHARED_BOOTLOADER_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
  BLStatusOk,
  BLStatusPending,
  BLStatusError,
} ps2plus_bootloader_status;

typedef enum {
  BLRecordTypeStart,
  BLRecordTypeData,
  BLRecordTypeEnd,
} ps2plus_bootloader_update_record_type;

typedef enum {
  BENoError,
  BEUnspecifiedError,
  BEInvalidChecksum,
  BEInvalidAddress,
} ps2plus_bootloader_error;

typedef struct {
  ps2plus_bootloader_update_record_type type;
  uint32_t target_address;
  uint8_t data[16];
  uint8_t data_length;
  uint8_t data_checksum;
} ps2plus_bootloader_update_record;

uint8_t ps2plus_bootloader_update_record_calculate_checksum(ps2plus_bootloader_update_record *record);

bool ps2plus_bootloader_update_record_validate_checksum(ps2plus_bootloader_update_record *record);

#endif	/* SHARED_BOOTLOADER_H */
