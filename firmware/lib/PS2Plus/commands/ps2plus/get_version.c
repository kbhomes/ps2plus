#include "../command.h"

const uint8_t PAYLOAD_FIRMWARE_VERSION[] = { 0x03, 0x01, 0x00 };
const uint8_t PAYLOAD_MICROCONTROLLER_VERSION[] = { 0x05, 11, 'P', 'I', 'C', '1', '8', 'F', '4', '6', 'K', '4', '2' };
const uint8_t PAYLOAD_CONFIGURATION_VERSION[] = { 0x03, 0x01, 0x00 };

typedef enum PS2PlusVersion {
  PS2PlusVersion_Firmware = 0,
  PS2PlusVersion_Microcontroller = 1,
  PS2PlusVersion_Configuration = 2,
  NUM_PS2PLUS_VERSION_IDENTIFIERS,
} PS2PlusVersion;

struct {
  PS2PlusVersion version_id;
  uint8_t *version_payload;
  size_t version_payload_length;
} gv_memory;

command_result gv_initialize(volatile command_packet *packet, controller_state *state) {
  // Reset the version identifier that the configurator has requested
  gv_memory.version_id = PS2PlusVersion_Firmware;
  gv_memory.version_payload = PAYLOAD_FIRMWARE_VERSION;
  gv_memory.version_payload_length = sizeof(PAYLOAD_FIRMWARE_VERSION);
  
  return CRInitialized;
}

command_result gv_process(volatile command_packet *packet, controller_state *state) {
  if (packet->data_index == 0) {
    packet->write(0x00);
  } else {
    if (packet->command_index == 0) {
      gv_memory.version_id = packet->command_byte;
      
      switch (gv_memory.version_id) {
        case PS2PlusVersion_Firmware:
          packet->write(0x03);
          gv_memory.version_payload = PAYLOAD_FIRMWARE_VERSION;
          gv_memory.version_payload_length = sizeof(PAYLOAD_FIRMWARE_VERSION);
          break;
        
        case PS2PlusVersion_Microcontroller:
          packet->write(0x05);
          gv_memory.version_payload = PAYLOAD_MICROCONTROLLER_VERSION;
          gv_memory.version_payload_length = sizeof(PAYLOAD_MICROCONTROLLER_VERSION);
          break;

        case PS2PlusVersion_Configuration:
          packet->write(0x03);
          gv_memory.version_payload = PAYLOAD_CONFIGURATION_VERSION;
          gv_memory.version_payload_length = sizeof(PAYLOAD_CONFIGURATION_VERSION);
          break;
      }
    } else {
      packet->write(gv_memory.version_payload[packet->data_index - 1]);
    }
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