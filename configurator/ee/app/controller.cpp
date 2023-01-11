#include "controller.h"

#include "libps2plman.h"
#include "ui/pad.h"

#include <cstring>
#include <cstdio>
#include <shared/versions.h>

namespace PS2Plus::App {
bool Controller::Connect() {
  PS2Plus::Gamepad::Stop(port_);
  connected_ = true;

  if (connected_ && !LoadVersions()) {
    connected_ = false;
  }

  if (connected_ && !LoadConfiguration()) {
    connected_ = false;
  }

  PS2Plus::Gamepad::Start(port_);
  return connected_;
}

bool Controller::LoadVersions() {
  int ret;

  if ((ret = ps2plman_get_firmware_version(&versions_.firmware)) != PS2PLMAN_RET_OK) {
    printf("Error retrieving PS2+ firmware version: %d\n", ret);
    connected_ = false;
  }

  if ((ret = ps2plman_get_microcontroller_version(versions_.microcontroller, NULL, sizeof(versions_.microcontroller) - 1)) !=
      PS2PLMAN_RET_OK) {
    printf("Error retrieving PS2+ microcontroller version: %d\n", ret);
    connected_ = false;
  }

  if ((ret = ps2plman_get_configuration_version(&versions_.configuration)) != PS2PLMAN_RET_OK) {
    printf("Error retrieving PS2+ configuration version: %d\n", ret);
    connected_ = false;
  }

  versions_.Dump();

  return false;
}

bool Controller::LoadConfiguration() {
  for (ConfigurationId i = 0; i < NUM_CUSTOM_CONFIGURATIONS; i++) {
    ps2plman_get_configuration(i, configuration_.Field(i));
  }

  return true;
}

std::string ControllerVersion::FirmwareString() const {
  std::string out;
  out += "v" + std::to_string(VERSION_MAJOR(firmware));
  out += "." + std::to_string(VERSION_MINOR(firmware));
  out += "." + std::to_string(VERSION_PATCH(firmware));

  int metadata = VERSION_METADATA(firmware);
  switch (metadata) {
  case VERSION_METADATA_DEV:
    out += "-dev";
    break;

  case VERSION_METADATA_LATEST:
    out += "-latest";
    break;
  }

  return out;
}

void ControllerVersion::Dump() {
  std::printf("[versions] firmware = %s\n", FirmwareString().c_str());
  std::printf("[versions] microcontroller = %s\n", microcontroller);
  std::printf("[versions] configuration = 0x%02x\n", configuration);
}

void Controller::MockData() {
  // Setup controller versions
  connected_ = true;
  versions_.firmware = VERSION64(1, 12, 0, VERSION_METADATA_DEV);
  versions_.configuration = 1;
  std::strncpy(versions_.microcontroller, "PIC18F46K42", 12);

  // Setup controller configurations
  primitive_data_initialize_boolean(configuration_.Field(CONFIGURATION_ID(enable_button_remapping)), false);
  for (size_t i = 0; i < NUM_DIGITAL_BUTTONS; i++) {
    ConfigurationId button_config_id = CONFIGURATION_ID(button_remapping) + i;
    primitive_data_initialize_uint8(configuration_.Field(button_config_id), i);
  }

  primitive_data_initialize_boolean(configuration_.Field(CONFIGURATION_ID(enable_joystick_axis_range_remapping)), false);
  for (size_t i = 0; i < NUM_JOYSTICK_AXIS_RANGES; i++) {
    ConfigurationId remapping_config_id = CONFIGURATION_ID(joystick_axis_range_remapping) + i;
    uint8_t remapping_value = (i % 3 == 0) ? 0 : (i % 3 == 1) ? 127 : 255;
    primitive_data_initialize_uint8(configuration_.Field(remapping_config_id), remapping_value);
  }

  primitive_data_initialize_uint8(configuration_.Field(CONFIGURATION_ID(joystick_deadzone_left)), 0);
  primitive_data_initialize_uint8(configuration_.Field(CONFIGURATION_ID(joystick_deadzone_right)), 0);
  primitive_data_initialize_boolean(configuration_.Field(CONFIGURATION_ID(joystick_digital_enable_left)), false);
  primitive_data_initialize_boolean(configuration_.Field(CONFIGURATION_ID(joystick_digital_enable_right)), false);
  primitive_data_initialize_uint8(configuration_.Field(CONFIGURATION_ID(joystick_digital_threshold_left)), 0x40);
  primitive_data_initialize_uint8(configuration_.Field(CONFIGURATION_ID(joystick_digital_threshold_right)), 0x40);
}
} // namespace PS2Plus::App