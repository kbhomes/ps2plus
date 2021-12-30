#include "../platform.h"
#include "../controller_state.h"

typedef enum {
  CRInitialized,
  CRProcessing,
  CRCompleted,
  CRError,
} command_result;

typedef struct {
  /**
   * @brief Command ID for this packet (e.g.: 42h for the main polling command)
   */
  uint8_t id;

  /**
   * @brief Most recently read byte from the console
   */
  uint8_t command_byte;

  /**
   * @brief Index of the most recently read COMMAND byte in the command packet, not including the 3-byte header
   */
  uint8_t command_index;

  /**
   * @brief Index of the to-be-written DATA byte in the command packet, not including the 3-byte header
   */
  uint8_t data_index;

  /**
   * @brief Write function to send a byte back to the console
   */
  void (*write)(uint8_t data);
} command_packet;

typedef struct {
  uint8_t id;
  command_result (*initialize)(controller_state *);
  command_result (*process)(command_packet *, controller_state *);

  // TODO: Add a `finalize` method that can receive the final command byte of the transmission
  //       (will help to correcltly implement the map motors command with all 6 actuator channels)
} command_processor;

// All available command processors
extern command_processor initialize_pressure_sensor_command;
extern command_processor button_inclusions_command;
extern command_processor main_polling_command;
extern command_processor enter_exit_config_command;
extern command_processor switch_digital_analog_mode_command;
extern command_processor status_info_command;
extern command_processor device_descriptor_46_command;
extern command_processor device_descriptor_47_command;
extern command_processor device_descriptor_4c_command;
extern command_processor map_motors_command;
extern command_processor configure_analog_response_command;

command_processor *command_processors[] = {
  &initialize_pressure_sensor_command,    // 40h
  &button_inclusions_command,             // 41h
  &main_polling_command,                  // 42h
  &enter_exit_config_command,             // 43h
  &switch_digital_analog_mode_command,    // 44h
  &status_info_command,                   // 45h
  &device_descriptor_46_command,          // 46h
  &device_descriptor_47_command,          // 47h
  &device_descriptor_4c_command,          // 4Ch
  &map_motors_command,                    // 4Dh
  &configure_analog_response_command,     // 4Fh
};