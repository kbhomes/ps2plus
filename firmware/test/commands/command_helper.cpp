#include "command_helper.h"

const size_t OUTPUT_BUFFER_SIZE = 255;
uint8_t output_buffer[OUTPUT_BUFFER_SIZE] = { 0x00 };
int output_index = 0;
bool helper_debug_enabled = false;

void print_hex_array(const uint8_t *array, size_t length) {
  for (size_t i = 0; i < length; i++) {
    printf("%02X ", array[i]);
  }
  printf("\n");
}

/**
 * @brief Stub function that acts as a command processor's write function in order to capture its output into a byte array
 */
void packet_write_stub(uint8_t value) {
  output_buffer[output_index++] = value;
}

void helper_initialize() {
  // Reset the output buffer
  output_index = 0;
  for (size_t i = 0; i < OUTPUT_BUFFER_SIZE; i++) {
    output_buffer[i] = 0;
  }

  // Reset helper flags
  helper_debug_enabled = false;
}

void helper_enable_debug(bool value) {
  helper_debug_enabled = value;
}

uint8_t *helper_run_command(controller_state *state, uint8_t command_id, const uint8_t *command_bytes, size_t command_length) {
  // Keep track of where the data output for this command begins
  int initial_index = output_index;

  // Create the command packet which will contain the command bytes
  command_packet packet = {
    .id = command_id,
    .command_byte = 0x00, // Byte immediately preceding the payload is always 0x00
    .command_index = -1,
    .data_index = 0,
    .write = &packet_write_stub,
  };

  // Find the correct command processor for the given ID
  command_processor *processor = NULL;
  for (size_t i = 0; command_processors[i] != NULL; i++) {
    if (command_processors[i]->id == command_id) {
      processor = command_processors[i];
    }
  } 

  if (processor == NULL) {
    char buffer[255];
    sprintf(buffer, "Could not find command processor for ID: %x", command_id);
    TEST_FAIL_MESSAGE(buffer);
  }

  // Initialize the command processor with the controller state
  processor->initialize(state);

  for (size_t i = 0; i < command_length; i++) {
    // All results should be `CRProcessing` (ongoing command), while the last should be `CRCompleted`
    command_result expected_result = (i + 1 == command_length) ? CRCompleted : CRProcessing;
    command_result actual_result = processor->process(&packet, state);
    TEST_ASSERT_EQUAL(expected_result, actual_result);

    // Allow the command processor to read and write the next byte
    packet.command_index++;
    packet.command_byte = command_bytes[packet.command_index];
    packet.data_index++;
  }

  if (helper_debug_enabled) {
    // Debug!
    printf("TX: ");
    print_hex_array(command_bytes, command_length);
    printf("RX: ");
    print_hex_array(&output_buffer[initial_index], command_length);
  }

  return &output_buffer[initial_index];
}