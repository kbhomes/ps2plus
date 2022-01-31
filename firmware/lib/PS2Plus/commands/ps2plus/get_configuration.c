#include "../command.h"

#include <utilities/primitive_data.h>

struct {
  uint16_t requested_configuration;
  primitive_data *value;
  uint8_t value_serialized[128];
  uint8_t value_serialized_length;
} gc_memory;

command_result gc_initialize(volatile command_packet *packet, controller_state *state) {
  gc_memory.requested_configuration = 0;
  gc_memory.value = &primitive_data_unknown;
  gc_memory.value_serialized_length = primitive_data_length(gc_memory.value);
  primitive_data_serialize(gc_memory.value, gc_memory.value_serialized);
  
  return CRInitialized;
}

command_result gc_process(volatile command_packet *packet, controller_state *state) {
  if (packet->command_index == 0) {
    // Read the lower byte of the requested configuration
    gc_memory.requested_configuration = packet->command_byte;
  } else if (packet->command_index == 1) {
    // Read the upper byte of the requested configuration
    gc_memory.requested_configuration |= packet->command_byte << 8;

    // Select the requested configuration
    if (gc_memory.requested_configuration < NUM_CUSTOM_CONFIGURATIONS) {
      gc_memory.value = &state->custom_config.all[gc_memory.requested_configuration];
    } else {
      gc_memory.value = &primitive_data_unknown;
    }
    
    // Convert the requested configuration to a deliverable payload
    gc_memory.value_serialized_length = primitive_data_length(gc_memory.value);
    primitive_data_serialize(gc_memory.value, gc_memory.value_serialized);
  }

  if (packet->data_index < 2) {
    packet->write(0x00);
  } else {
    // Write the serialized value
    packet->write(gc_memory.value_serialized[packet->data_index - 2]);
  }

  // If the final byte hasn't been written, mark this command as still processing
  if (packet->data_index + 1 != (2 + gc_memory.value_serialized_length)) {
    return CRProcessing;
  }

  return CRCompleted;
}

command_processor command_ps2plus_get_configuration = {
    .id = 0x71,
    .initialize = &gc_initialize,
    .process = &gc_process,
};