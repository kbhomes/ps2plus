#include "../command.h"
#include "shared/versions.h"

typedef enum PS2PlusVersion {
  PS2PlusVersion_Firmware = 0,
  PS2PlusVersion_Microcontroller = 1,
  PS2PlusVersion_Configuration = 2,
  NUM_PS2PLUS_VERSION_IDENTIFIERS,
} PS2PlusVersion;

struct {
  PS2PlusVersion version_id;
  uint8_t version_payload[128];
  size_t version_payload_length;
} gv_memory;

command_result gv_initialize(volatile command_packet *packet, controller_state *state) {
  // Reset the version identifier that the configurator has requested
  gv_memory.version_id = -1;
  primitive_data_serialize(&primitive_data_unknown, &gv_memory.version_payload);
  gv_memory.version_payload_length = primitive_data_length(&primitive_data_unknown);
  
  return CRInitialized;
}

command_result gv_process(volatile command_packet *packet, controller_state *state) {
  if (packet->data_index == 0) {
    packet->write(0x00);
  } else {
    if (packet->command_index == 0) {
      gv_memory.version_id = packet->command_byte;
      
      switch (gv_memory.version_id) {
#ifdef PS2PLUS_FIRMWARE
        case PS2PlusVersion_Firmware:
          primitive_data_serialize(&state->versions.firmware, &gv_memory.version_payload);
          gv_memory.version_payload_length = primitive_data_length(&state->versions.firmware);
          break;
        
        case PS2PlusVersion_Microcontroller:
          primitive_data_serialize(&state->versions.microcontroller, &gv_memory.version_payload);
          gv_memory.version_payload_length = primitive_data_length(&state->versions.microcontroller);
          break;
        
        case PS2PlusVersion_Configuration:
          primitive_data_serialize(&state->versions.configuration, &gv_memory.version_payload);
          gv_memory.version_payload_length = primitive_data_length(&state->versions.configuration);
          break;
#endif
      }
    }
    
    packet->write(gv_memory.version_payload[packet->data_index - 1]);
  }

  // If the final byte hasn't been written, mark this command as still processing
  if (packet->data_index + 1 != (1 + gv_memory.version_payload_length)) {
    return CRProcessing;
  }

  return CRCompleted;
}

command_processor command_ps2plus_get_version = {
    .id = 0x70,
    .initialize = &gv_initialize,
    .process = &gv_process,
};