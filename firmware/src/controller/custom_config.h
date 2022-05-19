#ifdef PS2PLUS_FIRMWARE

#ifndef CONTROLLER_CUSTOM_CONFIG_H
#define CONTROLLER_CUSTOM_CONFIG_H

#include <stdbool.h>
#include <stdint.h>

#include <shared/config.h>
#include <shared/primitive_data.h>

typedef struct {
  /**
   * @brief Whether custom configurations are globally enabled (not stored in storage)
   */
  bool enabled;

  /**
   * @brief Configuration version as saved in internal storage. On startup, a mismatch
   *        between the firmware's configuration version and the stored version indicates
   *        a backwards-incompatible configuration change that will require resetting. 
   */
  uint16_t configuration_version;

  /**
   * @brief Configuration values that can be serialized for transfer between the firmware
   *        and the PS2+ configurator application
   */
  union {
    ps2plus_configuration values;
    primitive_data all[NUM_CUSTOM_CONFIGURATIONS];
  };
} controller_custom_config;

void controller_custom_config_initialize(controller_custom_config *config);
void controller_custom_config_load(controller_custom_config *config);
void controller_custom_config_save(controller_custom_config *config);
void controller_custom_config_erase(controller_custom_config *config);
void controller_custom_config_dump(controller_custom_config *config);

#endif /* CONTROLLER_CUSTOM_CONFIG_H */

#endif /* PS2PLUS_FIRMWARE */