#include "controller-configuration.h"

#include <cstdio>
#include <cstring>

#include <toml/toml.hpp>

namespace PS2Plus::App {
Configuration::Configuration(uint16_t version) : version_(version) { std::memset(&config_, 0, sizeof(config_)); }

Configuration::Configuration(uint16_t version, ps2plus_configuration *config) : version_(version) {
  std::memcpy(&config_, config, sizeof(ps2plus_configuration));
}

std::map<ConfigurationId, primitive_data *> Configuration::GetDifferences(Configuration& other) {
  if (version_ != other.version_) {
    return std::map<ConfigurationId, primitive_data *>();
  }

  std::map<ConfigurationId, primitive_data *> differences;
  primitive_data *all_configs_old = reinterpret_cast<primitive_data *>(&config_);
  primitive_data *all_configs_new = reinterpret_cast<primitive_data *>(&other.config_);

  for (size_t i = 0; i < NUM_CUSTOM_CONFIGURATIONS; i++) {
    if (!primitive_data_equals(&all_configs_old[i], &all_configs_new[i])) {
      differences[i] = &all_configs_new[i];
    }
  }

  return differences;
}

primitive_data *Configuration::Field(ConfigurationId id) {
  primitive_data *all_configs = reinterpret_cast<primitive_data *>(&config_);
  return &all_configs[id];
}

ps2plus_configuration& Configuration::GetRawStructure() { return config_; }

bool Configuration::operator==(Configuration& other) { return version_ == other.version_ && GetDifferences(other).empty(); }

bool Configuration::operator!=(Configuration& other) { return !operator==(other); }

void Configuration::Export(std::ostream& out) {
  auto table = toml::table{{"__version__", version_}};
  auto table_button_remapping = toml::table{};
  auto table_jsar_remapping = toml::table{};

  table.insert("enable_button_remapping", config_.enable_button_remapping.boolean);
  for (uint8_t b = 0; b < NUM_DIGITAL_BUTTONS; b++) {
    ps2plus_controller_digital_button button_id = (ps2plus_controller_digital_button)b;
    ps2plus_controller_digital_button remapped_id = (ps2plus_controller_digital_button)config_.button_remapping[b].uint8;
    const char *button_name = ps2plus_controller_digital_button_name(button_id);
    const char *remapped_name = ps2plus_controller_digital_button_name(remapped_id);
    table_button_remapping.insert(button_name, remapped_name);
  }
  table.insert("button_remapping", table_button_remapping);

  table.insert("enable_joystick_axis_range_remapping", config_.enable_joystick_axis_range_remapping.boolean);
  for (size_t a = 0; a < NUM_JOYSTICK_AXIS_RANGES; a++) {
    ps2plus_controller_joystick_axis_range axis_range_id = (ps2plus_controller_joystick_axis_range)a;
    const char *axis_range_name = ps2plus_controller_joystick_axis_range_name(axis_range_id);
    uint8_t remapped_value = config_.joystick_axis_range_remapping[a].uint8;
    table_jsar_remapping.insert(axis_range_name, remapped_value);
  }
  table.insert("joystick_axis_range_remapping", table_jsar_remapping);

  table.insert("joystick_deadzone_left", config_.joystick_deadzone_left.uint8);
  table.insert("joystick_deadzone_right", config_.joystick_deadzone_right.uint8);
  table.insert("joystick_digital_enable_left", config_.joystick_digital_enable_left.boolean);
  table.insert("joystick_digital_enable_right", config_.joystick_digital_enable_right.boolean);
  table.insert("joystick_digital_threshold_left", config_.joystick_digital_threshold_left.uint8);
  table.insert("joystick_digital_threshold_right", config_.joystick_digital_threshold_right.uint8);

  out << table;
}

Configuration Configuration::Import(std::istream& in) {
  auto table = toml::parse(in);
  auto table_button_remapping = table["button_remapping"];
  auto table_jsar_remapping = table["joystick_axis_range_remapping"];

  Configuration wrapper(table["__version__"].value_or(0));

  wrapper.config_.enable_button_remapping.boolean = table["enable_button_remapping"].value_or(false);
  for (uint8_t b = 0; b < NUM_DIGITAL_BUTTONS; b++) {
    ps2plus_controller_digital_button button_id = (ps2plus_controller_digital_button)b;
    std::string button_name(ps2plus_controller_digital_button_name(button_id));
    std::string remapped_name(table_button_remapping[button_name].value_or(button_name));
    ps2plus_controller_digital_button remapped_id = ps2plus_controller_digital_button_from_name(remapped_name.c_str());
    wrapper.config_.button_remapping[b].uint8 = remapped_id;
  }

  wrapper.config_.enable_joystick_axis_range_remapping.boolean = table["enable_joystick_axis_range_remapping"].value_or(false);
  for (size_t a = 0; a < NUM_JOYSTICK_AXIS_RANGES; a++) {
    ps2plus_controller_joystick_axis_range axis_range_id = (ps2plus_controller_joystick_axis_range)a;
    std::string axis_range_name(ps2plus_controller_joystick_axis_range_name(axis_range_id));
    uint8_t remapped_value = table_jsar_remapping[axis_range_name].value_or(0x7F);
    wrapper.config_.joystick_axis_range_remapping[a].uint8 = remapped_value;
  }

  wrapper.config_.joystick_deadzone_left.uint8 = table["joystick_deadzone_left"].value_or(0);
  wrapper.config_.joystick_deadzone_right.uint8 = table["joystick_deadzone_right"].value_or(0);
  wrapper.config_.joystick_digital_enable_left.boolean = table["joystick_digital_enable_left"].value_or(false);
  wrapper.config_.joystick_digital_enable_right.boolean = table["joystick_digital_enable_right"].value_or(false);
  wrapper.config_.joystick_digital_threshold_left.uint8 = table["joystick_digital_threshold_left"].value_or(0);
  wrapper.config_.joystick_digital_threshold_right.uint8 = table["joystick_digital_threshold_right"].value_or(0);

  return wrapper;
}

} // namespace PS2Plus::App