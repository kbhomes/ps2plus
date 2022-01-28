#include "command.h"

const uint8_t IPS_RESPONSE_BYTES[6] = { 0x00, 0x00, 0x02, 0x00, 0x00, 0x5A };

command_result ips_initialize(volatile command_packet *packet, controller_state *state) {
  // No initialization or memory state management needed
  return CRInitialized;
}

command_result ips_process(volatile command_packet *packet, controller_state *state) {
  packet->write(IPS_RESPONSE_BYTES[packet->data_index]);

  // If the final byte hasn't been written, mark this command as still processing
  if (packet->data_index + 1 != 6) {
    return CRProcessing;
  }

  return CRCompleted;
}

command_processor initialize_pressure_sensor_command = {
    .id = 0x40,
    .initialize = &ips_initialize,
    .process = &ips_process,
};