#pragma once

#include <iostream>
#include <map>
#include <string>

#include <shared/config.h>

namespace PS2Plus::App {
using ConfigurationId = uint16_t;

class Configuration {
public:
  // Blank configuration
  Configuration(uint16_t version = 0);

  // Constructed from a native PS2+ configuration structure
  Configuration(uint16_t version, ps2plus_configuration *config);

  // Copy constructor to clone a configuration
  Configuration(Configuration& config) : Configuration(config.version_, &config.config_) {}

  // Compute the configuration values that differ between this ("old") and the other ("new")
  std::map<ConfigurationId, primitive_data *> GetDifferences(Configuration& other);

  // Get the configuration primitive data for the given ID (use `CONFIGURATION_ID` macro)
  primitive_data *Field(ConfigurationId id);

  // Directly access the underlying configuration structure
  ps2plus_configuration& GetRawStructure();

  bool operator==(Configuration& other);
  bool operator!=(Configuration& other);

  // Reset configuration values to their defaults
  void Reset();

  // Write this configuration in TOML format to the given output stream
  void Export(std::ostream& out);

  // Create a new configuration object from the TOML format in the given input stream
  static Configuration Import(std::istream& in);

  auto version() const { return version_; }

private:
  uint16_t version_;
  ps2plus_configuration config_;
};
} // namespace PS2Plus::App