#include "command.h"

// Since the polling behavior of the 43h command is nearly identical to the 42h command (main polling), 
// its processing is implemented as part of the main polling command. Whether to actuate the rumble 
// motors (as in 42h) or enter/exit config mode (as in 43h) is differented based on the command ID.
command_processor enter_exit_config_command = {
  .id = 0x43,
  .initialize = main_polling_command.initialize,
  .process = main_polling_command.process,
};