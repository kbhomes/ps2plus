#include "suite.h"

void test_command_ps2plus_disable_enable_configuration() {
  bool values[] = { false, true };

  for (size_t oi = 0; oi < sizeof(values); oi++) {
    for (size_t ni = 0; ni < sizeof(values); ni++) {
      bool original_enabled = values[oi];
      bool new_enabled = values[ni];

      // Set the original value of the state's custom config 
      state.custom_config.enabled = original_enabled;

      // Command input is set depending on the desired (new) state
      uint8_t command_input[] = { new_enabled ? 0xFF : 0x00 };

      // Expected output is the previous/original state
      uint8_t expected_output[] = { original_enabled ? 0xFF : 0x00 };
      uint8_t *actual_output = helper_run_command(command_ps2plus_disable_enable_configuration.id, command_input, sizeof(command_input));
      TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, actual_output, sizeof(command_input));
      
      // The state's custom config should now be the new value
      TEST_ASSERT_EQUAL(new_enabled, state.custom_config.enabled);
    }
  }
}