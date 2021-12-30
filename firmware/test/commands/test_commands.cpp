#include <platform.h>
#include <commands/command.h>
#include <stdio.h>
#include <unity.h>

// Status info command payload
const size_t PAYLOAD_SIZE = 6;
const uint8_t COMMAND_INPUT[PAYLOAD_SIZE] = { 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, };

const size_t output_buffer_size = 255;
uint8_t output_buffer[output_buffer_size] = { 0x00 };
int output_index = 0;

controller_state state = { 
  .input = {
    .digital_buttons = { 0 },
    .joysticks = { 0 },
  },
  .analog_mode = CMDigital,
  .analog_mode_locked = false,
  .config_mode = false,
  .rumble_motor_small = { .mapping = 0xFF, .value = 0x00 },
  .rumble_motor_large = { .mapping = 0xFF, .value = 0x00 },
};

void packet_write_stub(uint8_t value) {
  output_buffer[output_index++] = value;
}

uint8_t *run_command(controller_state *state, uint8_t command_id, const uint8_t *command_bytes, size_t command_length) {
  // Reset the output buffer
  output_index = 0;
  for (size_t i = 0; i < output_buffer_size; i++) {
    output_buffer[i] = 0;
  }

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

  return output_buffer;
}

void setUp() {
  // Initialize the controller input
  controller_input_initialize(&state.input);
}

void test_status_info_command_digital_mode() {
  state.analog_mode = CMDigital;

  uint8_t expected_output[PAYLOAD_SIZE] = { 0x03, 0x02, 0x00, 0x02, 0x01, 0x00, };
  uint8_t *actual_output = run_command(&state, 0x45, COMMAND_INPUT, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, PAYLOAD_SIZE);
}

void test_status_info_command_analog_mode() {
  state.analog_mode = CMAnalog;

  uint8_t expected_output[PAYLOAD_SIZE] = { 0x03, 0x02, 0x01, 0x02, 0x01, 0x00, };
  uint8_t *actual_output = run_command(&state, 0x45, COMMAND_INPUT, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, PAYLOAD_SIZE);
}

void test_status_info_command_analog_full_mode() {
  state.analog_mode = CMAnalogFull;

  uint8_t expected_output[PAYLOAD_SIZE] = { 0x03, 0x02, 0x01, 0x02, 0x01, 0x00, };
  uint8_t *actual_output = run_command(&state, 0x45, COMMAND_INPUT, PAYLOAD_SIZE);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, PAYLOAD_SIZE);
}

int main(void) {
  platform_init();

  UNITY_BEGIN();
  RUN_TEST(&test_status_info_command_digital_mode);
  RUN_TEST(&test_status_info_command_analog_mode);
  RUN_TEST(&test_status_info_command_analog_full_mode);
  UNITY_END();
}