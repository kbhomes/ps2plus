#include "libps2plman.h"

#include <string.h>

static SifRpcClientData_t rpc_client_data;
static ps2plman_rpc_packet rpc_packet;
static primitive_data response;

int ps2plman_init() {
  // Attempt to bind to the PS2+ Updater's RPC server
  do {
    if (SifBindRpc(&rpc_client_data, PS2PLMAN_RPC_BIND_ID, 0) < 0) {
      return PS2PLMAN_RET_ERROR_RPC;
    }
    nopdelay();
  } while(!rpc_client_data.server);

  return 0;
}

// Get version commands
int ps2plman_get_version(uint8_t id, primitive_data *out) {
  rpc_packet.command = PS2Plus_GetVersion;
  rpc_packet.get_version.version_id = id;
  
  int ret = SifCallRpc(&rpc_client_data, 0, 0, &rpc_packet, sizeof(ps2plman_rpc_packet), &rpc_packet, sizeof(ps2plman_rpc_packet), NULL, NULL);
  if (ret < 0) {
    return PS2PLMAN_RET_ERROR_RPC;
  }

  if (!rpc_packet.ok) {
    return PS2PLMAN_RET_ERROR_CONTROLLER;
  }

  // Copy the response to the outbound variable
  memcpy(out, &rpc_packet.get_version.version_response, sizeof(primitive_data));
  
  // "Unbox" the array data that was put in the packet buffer
  if (rpc_packet.get_version.version_response.type == PDT_Array) {
    out->array.data = (uint8_t *)rpc_packet.get_version.version_buffer;
  }

  return PS2PLMAN_RET_OK;
}

int ps2plman_get_firmware_version(uint64_t *out) {
  int ret = ps2plman_get_version(0x00, &response);

  if (response.type != PDT_Uint64) {
    return PS2PLMAN_RET_ERROR_INCORRECT_TYPE;
  } 
  
  if (out) {
    *out = response.uint64;
  }

  return ret;
}

int ps2plman_get_microcontroller_version(char *out, size_t *size, size_t max_size) {
  int ret = ps2plman_get_version(0x01, &response);

  if (response.type != PDT_Array) {
    return PS2PLMAN_RET_ERROR_INCORRECT_TYPE;
  } 
  
  if (size) {
    *size = response.array.length;
  }

  if (out) {
    size_t copy_size = (max_size > response.array.length) ? response.array.length : max_size;
    memcpy(out, response.array.data, copy_size);
  }

  return ret;
}

int ps2plman_get_configuration_version(uint16_t *out) {
  int ret = ps2plman_get_version(0x02, &response);

  if (response.type != PDT_Uint16) {
    return PS2PLMAN_RET_ERROR_INCORRECT_TYPE;
  } 
  
  if (out) {
    *out = response.uint16;
  }

  return ret;
}


// Get configuration value commands
int ps2plman_get_configuration(uint8_t id, primitive_data *out) {
  rpc_packet.command = PS2Plus_GetConfiguration;
  rpc_packet.get_configuration.configuration_id = id;
  
  int ret = SifCallRpc(&rpc_client_data, 0, 0, &rpc_packet, sizeof(ps2plman_rpc_packet), &rpc_packet, sizeof(ps2plman_rpc_packet), NULL, NULL);
  if (ret < 0) {
    return PS2PLMAN_RET_ERROR_RPC;
  }

  if (!rpc_packet.ok) {
    return PS2PLMAN_RET_ERROR_CONTROLLER;
  }

  // Copy the response to the outbound variable
  memcpy(out, &rpc_packet.get_version.version_response, sizeof(primitive_data));
  
  // "Unbox" the array data that was put in the packet buffer
  if (rpc_packet.get_version.version_response.type == PDT_Array) {
    out->array.data = (uint8_t *)rpc_packet.get_version.version_buffer;
  }

  return PS2PLMAN_RET_OK;
}

int ps2plman_get_configuration_bool(uint8_t id, bool *out) {
  int ret = ps2plman_get_configuration(id, &response);

  if (response.type != PDT_Boolean) {
    return PS2PLMAN_RET_ERROR_INCORRECT_TYPE;
  } 
  
  if (out) {
    *out = response.boolean;
  }

  return ret;
}

int ps2plman_get_configuration_uint8(uint8_t id, uint8_t *out) {
  int ret = ps2plman_get_configuration(id, &response);

  if (response.type != PDT_Uint8) {
    return PS2PLMAN_RET_ERROR_INCORRECT_TYPE;
  } 
  
  if (out) {
    *out = response.uint8;
  }

  return ret;
}

int ps2plman_get_configuration_uint16(uint8_t id, uint16_t *out) {
  int ret = ps2plman_get_configuration(id, &response);

  if (response.type != PDT_Uint8) {
    return PS2PLMAN_RET_ERROR_INCORRECT_TYPE;
  } 
  
  if (out) {
    *out = response.uint16;
  }

  return ret;
}

int ps2plman_get_configuration_uint32(uint8_t id, uint32_t *out) {
  int ret = ps2plman_get_configuration(id, &response);

  if (response.type != PDT_Uint32) {
    return PS2PLMAN_RET_ERROR_INCORRECT_TYPE;
  } 
  
  if (out) {
    *out = response.uint32;
  }

  return ret;
}

int ps2plman_get_configuration_array(uint8_t id, char *out, size_t *size, size_t max_size) {
  int ret = ps2plman_get_configuration(id, &response);

  if (response.type != PDT_Array) {
    return PS2PLMAN_RET_ERROR_INCORRECT_TYPE;
  } 
  
  if (size) {
    *size = response.array.length;
  }

  if (out) {
    size_t copy_size = (max_size > response.array.length) ? response.array.length : max_size;
    memcpy(out, response.array.data, copy_size);
  }

  return ret;
}


// Set configuration value commands
int ps2plman_set_configuration(uint8_t id, primitive_data *in) {
  rpc_packet.command = PS2Plus_SetConfiguration;
  rpc_packet.set_configuration.configuration_id = id;

  // Copy the inbound variable to the RPC packet
  memcpy(&rpc_packet.set_configuration.configuration_value, in, sizeof(primitive_data));
  
  // "Box" the array data by putting it into the packet buffer
  if (in->type == PDT_Array && in->array.data) {
    memcpy(&rpc_packet.set_configuration.configuration_buffer, in->array.data, in->array.length);
  }
  
  int ret = SifCallRpc(&rpc_client_data, 0, 0, &rpc_packet, sizeof(ps2plman_rpc_packet), &rpc_packet, sizeof(ps2plman_rpc_packet), NULL, NULL);
  if (ret < 0) {
    return PS2PLMAN_RET_ERROR_RPC;
  }

  if (!rpc_packet.ok) {
    return PS2PLMAN_RET_ERROR_CONTROLLER;
  }

  return PS2PLMAN_RET_OK;
}

int ps2plman_set_configuration_bool(uint8_t id, bool in) {
  primitive_data_initialize_boolean(&response, in);
  return ps2plman_set_configuration(id, &response);
}

int ps2plman_set_configuration_uint8(uint8_t id, uint8_t in) {
  primitive_data_initialize_uint8(&response, in);
  return ps2plman_set_configuration(id, &response);
}

int ps2plman_set_configuration_uint16(uint8_t id, uint16_t in) {
  primitive_data_initialize_uint16(&response, in);
  return ps2plman_set_configuration(id, &response);
}

int ps2plman_set_configuration_uint32(uint8_t id, uint32_t in) {
  primitive_data_initialize_uint32(&response, in);
  return ps2plman_set_configuration(id, &response);
}

int ps2plman_set_configuration_array(uint8_t id, char *in, size_t size) {
  primitive_data_initialize_array(&response, (uint8_t *)in, size);
  return ps2plman_set_configuration(id, &response);
}


// Other commands
int ps2plman_disable_enable_configuration(bool enable, bool *original) {
  rpc_packet.command = PS2Plus_DisableEnableConfiguration;
  
  int ret = SifCallRpc(&rpc_client_data, 0, 0, &rpc_packet, sizeof(ps2plman_rpc_packet), &rpc_packet, sizeof(ps2plman_rpc_packet), NULL, NULL);
  if (ret < 0) {
    return PS2PLMAN_RET_ERROR_RPC;
  }

  if (!rpc_packet.ok) {
    return PS2PLMAN_RET_ERROR_CONTROLLER;
  }

  if (original) {
    *original = rpc_packet.disable_enable_configuration.configuration_previous;
  }

  return PS2PLMAN_RET_OK;
}

int ps2plman_restore_configuration_defaults() {
  rpc_packet.command = PS2Plus_RestoreConfigurationDefaults;
  
  int ret = SifCallRpc(&rpc_client_data, 0, 0, &rpc_packet, sizeof(ps2plman_rpc_packet), &rpc_packet, sizeof(ps2plman_rpc_packet), NULL, NULL);
  if (ret < 0) {
    return PS2PLMAN_RET_ERROR_RPC;
  }

  if (!rpc_packet.ok) {
    return PS2PLMAN_RET_ERROR_CONTROLLER;
  }

  return PS2PLMAN_RET_OK;
}

int ps2plman_reboot_controller() {
  rpc_packet.command = PS2Plus_RebootController;
  
  int ret = SifCallRpc(&rpc_client_data, 0, 0, &rpc_packet, sizeof(ps2plman_rpc_packet), &rpc_packet, sizeof(ps2plman_rpc_packet), NULL, NULL);
  if (ret < 0) {
    return PS2PLMAN_RET_ERROR_RPC;
  }

  if (!rpc_packet.ok) {
    return PS2PLMAN_RET_ERROR_CONTROLLER;
  }

  return PS2PLMAN_RET_OK;
}

int ps2plman_bootloader_update_firmware_data(ps2plus_bootloader_update_record *record) {
  rpc_packet.command = PS2Plus_BootloaderUpdateFirmwareData;
  memcpy(&rpc_packet.bootloader_update_firmware_data.record, record, sizeof(*record));
  
  int ret = SifCallRpc(&rpc_client_data, 0, 0, &rpc_packet, sizeof(ps2plman_rpc_packet), &rpc_packet, sizeof(ps2plman_rpc_packet), NULL, NULL);
  if (ret < 0) {
    return PS2PLMAN_RET_ERROR_RPC;
  }

  if (!rpc_packet.ok) {
    return PS2PLMAN_RET_ERROR_CONTROLLER;
  }

  return PS2PLMAN_RET_OK;
}

int ps2plman_bootloader_query_firmware_update_status(ps2plus_bootloader_status *status, ps2plus_bootloader_error *error) {
  rpc_packet.command = PS2Plus_BootloaderQueryFirmwareUpdateStatus;
  
  int ret = SifCallRpc(&rpc_client_data, 0, 0, &rpc_packet, sizeof(ps2plman_rpc_packet), &rpc_packet, sizeof(ps2plman_rpc_packet), NULL, NULL);
  if (ret < 0) {
    return PS2PLMAN_RET_ERROR_RPC;
  }

  if (!rpc_packet.ok) {
    return PS2PLMAN_RET_ERROR_CONTROLLER;
  }

  *status = rpc_packet.bootloader_query_firmware_update_status.status;
  *error = rpc_packet.bootloader_query_firmware_update_status.error;

  return PS2PLMAN_RET_OK;
}