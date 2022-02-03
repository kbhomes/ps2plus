#ifndef EE_LIBPS2PLMAN_H
#define EE_LIBPS2PLMAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "irx_builtins.h"

#include <ps2plus_common.h>

#include <kernel.h>
#include <sifcmd.h>
#include <sifrpc.h>
#include <stdio.h>
#include <tamtypes.h>
#include <unistd.h>

#define PS2PLMAN_RET_OK 0
#define PS2PLMAN_RET_ERROR_RPC 1
#define PS2PLMAN_RET_ERROR_CONTROLLER 2
#define PS2PLMAN_RET_ERROR_INCORRECT_TYPE 3

int ps2plman_init();

// Get version commands
int ps2plman_get_version(uint8_t id, primitive_data *out);
int ps2plman_get_firmware_version(uint16_t *out);
int ps2plman_get_microcontroller_version(char *out, size_t *size, size_t max_size);
int ps2plman_get_configuration_version(uint16_t *out);

// Get configuration value commands
int ps2plman_get_configuration(uint8_t id, primitive_data *out);
int ps2plman_get_configuration_bool(uint8_t id, bool *out);
int ps2plman_get_configuration_uint8(uint8_t id, uint8_t *out);
int ps2plman_get_configuration_uint16(uint8_t id, uint16_t *out);
int ps2plman_get_configuration_uint32(uint8_t id, uint32_t *out);
int ps2plman_get_configuration_array(uint8_t id, char *out, size_t *size, size_t max_size);

// Set configuration value commands
int ps2plman_set_configuration(uint8_t id, primitive_data *in);
int ps2plman_set_configuration_bool(uint8_t id, bool in);
int ps2plman_set_configuration_uint8(uint8_t id, uint8_t in);
int ps2plman_set_configuration_uint16(uint8_t id, uint16_t in);
int ps2plman_set_configuration_uint32(uint8_t id, uint32_t in);
int ps2plman_set_configuration_array(uint8_t id, char *in, size_t size);

// Other commands
int ps2plman_disable_enable_configuration(bool enable, bool *original);
int ps2plman_restore_configuration_defaults();
int ps2plman_reboot_controller();

#ifdef __cplusplus
}
#endif

#endif /* EE_LIBPS2PLMAN_H */