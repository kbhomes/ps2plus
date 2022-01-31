#include "../command.h"

#include <utilities/primitive_data.h>

#define SC_TARGET_CONFIGURATION_VALID 0x00
#define SC_TARGET_CONFIGURATION_INVALID 0xFF

struct {
  uint16_t target_configuration;
  uint8_t target_status;
  primitive_data *target_value;
  uint8_t value_raw[128];
  uint8_t value_raw_length;
} sc_memory;

command_result sc_initialize(volatile command_packet *packet, controller_state *state) {
  sc_memory.target_configuration = 0;
  sc_memory.target_status = 0;
  sc_memory.target_value = NULL;
  sc_memory.value_raw_length = 2;

  return CRInitialized;
}

command_result sc_process(volatile command_packet *packet, controller_state *state) {
  if (packet->command_index == 0) {
    // Read the lower byte of the targeted configuration
    sc_memory.target_configuration = packet->command_byte;
  } else if (packet->command_index == 1) {
    // Read the upper byte of the targeted configuration
    sc_memory.target_configuration |= packet->command_byte << 8;

    // Determine whether the target is valid
    if (sc_memory.target_configuration < NUM_CUSTOM_CONFIGURATIONS) {
      sc_memory.target_status = SC_TARGET_CONFIGURATION_VALID;
      sc_memory.target_value = &state->custom_config.all[sc_memory.target_configuration];
      sc_memory.value_raw_length = primitive_data_length(sc_memory.target_value);
    } else {
      sc_memory.target_status = SC_TARGET_CONFIGURATION_INVALID;
    }
  } else {
    // Only do processing if the the target is valid
    if (sc_memory.target_value != NULL) {
      sc_memory.value_raw[packet->command_index - 2] = packet->command_byte;
      
      // The target configuration type and the incoming value type must match
      if (packet->command_index == 3 && sc_memory.target_value->type != packet->command_byte) {
        sc_memory.target_value = NULL;
        sc_memory.target_status = SC_TARGET_CONFIGURATION_INVALID;
      }

      // Array primitives require the extra byte to determine the length
      if (packet->command_index == 4 && sc_memory.target_value->type == PDT_Array) {
        sc_memory.target_value->array.length = packet->command_byte;
        sc_memory.value_raw_length = primitive_data_length(sc_memory.target_value);
      }
    }
  }

  if (packet->data_index < 2) {
    packet->write(0x00);
  } else {
    // Write the target validity status
    packet->write(sc_memory.target_status);
  }

  // If the final byte hasn't been written, mark this command as still processing
  if (packet->data_index + 1 != (2 + sc_memory.value_raw_length)) {
    return CRProcessing;
  }

  return CRCompleted;
}

void sc_finalize(volatile command_packet *packet, controller_state *state) {
  if (sc_memory.target_value != NULL) {
    // Read the final incoming byte
    sc_memory.value_raw[sc_memory.value_raw_length - 1] = packet->command_byte;

    // Deserialize the processed data into the target configuration
    primitive_data_deserialize(sc_memory.target_value, sc_memory.value_raw);
  }
}

command_processor command_ps2plus_set_configuration = {
    .id = 0x71,
    .initialize = &sc_initialize,
    .process = &sc_process,
    .finalize = &sc_finalize,
};