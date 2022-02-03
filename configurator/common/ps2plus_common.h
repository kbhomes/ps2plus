#ifndef PS2PLUS_COMMON_H
#define PS2PLUS_COMMON_H

#include <primitive_data.h>

#define PS2PLUPD_RPC_BIND_ID 0x7FFFA100
#define PS2PLUPD_THREAD_PRIO 46

typedef enum {
  PS2Plus_Init,
  PS2Plus_GetVersion,
  PS2Plus_GetConfiguration,
  PS2Plus_SetConfiguration,
  PS2Plus_DisableEnableConfiguration,
  PS2Plus_RestoreConfigurationDefaults,
  NUM_PS2PLUS_RPC_COMMANDS,
} ps2plus_rpc_command;

typedef struct {
  uint8_t version_id;
  primitive_data version_response;
  char version_buffer[32];
} ps2plus_rpc_command_get_version;

typedef struct {
  uint16_t configuration_id;
  primitive_data configuration_response;
  char configuration_buffer[32];
} ps2plus_rpc_command_get_configuration;

typedef struct {
  uint16_t configuration_id;
  primitive_data configuration_value;
} ps2plus_rpc_command_set_configuration;

typedef struct {
  uint16_t configuration_id;
  bool configuration_enable;
  bool configuration_previous;
} ps2plus_rpc_command_disable_enable_configuration;

typedef struct {
  // Empty struct -- no payload
} ps2plus_rpc_command_restore_configuration_defaults;

typedef union {
  struct {
    ps2plus_rpc_command command;
    union {
      ps2plus_rpc_command_get_version get_version;
      ps2plus_rpc_command_get_configuration get_configuration;
      ps2plus_rpc_command_set_configuration set_configuration;
      ps2plus_rpc_command_disable_enable_configuration disable_enable_configuration;
      ps2plus_rpc_command_restore_configuration_defaults restore_configuration_defaults;
    };
    bool ok;
  };
  uint8_t _[128];
} ps2plus_rpc_packet __attribute__((aligned(16)));

#endif