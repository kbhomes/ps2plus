#include "test_commands.h"

#define PS2PLUS_GC_COMMAND_ID 0x73

void test_command_ps2plus_disable_enable_configuration() {
  bool values[] = { false, true };

  for (size_t oi = 0; oi < sizeof(values); oi++) {
    for (size_t ni = 0; ni < sizeof(values); ni++) {
      // Set the original configuration 
      state.custom_config.enabled = values[oi];
      uint8_t command_input[] = { values[ni] ? 0xFF : 0x00 };
      uint8_t expected_output[] = { values[oi] ? 0xFF : 0x00 };
      uint8_t *actual_output = helper_run_command(&state, PS2PLUS_GC_COMMAND_ID, command_input, sizeof(command_input));
      TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, sizeof(command_input));
      TEST_ASSERT_EQUAL(values[ni], state.custom_config.enabled);
    }
  }
}