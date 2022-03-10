#ifdef PS2PLUS_BOOTLOADER

#ifndef CONTROLLER_BOOTLOADER_H
#define	CONTROLLER_BOOTLOADER_H

#include <stdbool.h>
#include <stdint.h>

#include <shared/bootloader.h>

typedef struct {
  /**
   * @brief Current status of the bootloader's update process
   * 
   * Defaults to "error" if the update process has not yet started.
   */
  ps2plus_bootloader_status status;
  
  /**
   * @brief Current error state of the bootloader
   * 
   * Defaults to "unspecified"; only sent to the console if the status is also "error".
   */
  ps2plus_bootloader_error error;
  
  struct {
    bool ready;
    ps2plus_bootloader_update_record record;
  } update;
} controller_bootloader;

#endif	/* CONTROLLER_BOOTLOADER_H */

#endif /* PS2PLUS_BOOTLOADER */