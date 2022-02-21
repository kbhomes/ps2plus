#ifndef SHARED_BOOTLOADER_H
#define	SHARED_BOOTLOADER_H

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
  BEUnspecifiedError,
  BEInvalidChecksum,
  BEInvalidAddress,
  BEInvalidFirstInstruction,
} ps2plus_bootloader_error;

#endif	/* SHARED_BOOTLOADER_H */
