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
} ps2plus_updater_rpc_command;

typedef union {
  struct {
    ps2plus_updater_rpc_command command;
    union {
      struct {
        uint8_t version_id;
        primitive_data version_response;
        char version_buffer[32];
      } command_get_version;

      struct {
        uint16_t configuration_id;
        primitive_data configuration_response;
        char configuration_buffer[32];
      } command_get_configuration;

      struct {
        uint16_t configuration_id;
        primitive_data configuration_value;
      } command_set_configuration;

      struct {
        uint16_t configuration_id;
        bool configuration_enable;
        bool configuration_previous;
      } command_disable_enable_configuration;

      struct {
        // Empty struct -- no payload
      } command_restore_configuration_defaults;
    };
  };
  uint8_t _[128];
} ps2plus_updater_rpc_packet __attribute__((aligned(16)));

#endif