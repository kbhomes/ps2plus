#pragma once

#include "controller-configuration.h"

#include <string>

namespace PS2Plus::App {
struct ControllerVersion {
  uint64_t firmware;
  char microcontroller[33];
  uint16_t configuration;

  std::string FirmwareString() const;
  void Dump();
};

class Controller {
public:
  Controller(int port) : port_(port) {}

  int port() const { return port_; }
  bool connected() const { return connected_; }
  Configuration& configuration() { return configuration_; }
  ControllerVersion& versions() { return versions_; }

  bool Connect();

  static Controller CreateMockedController(int port);

private:
  bool LoadVersions();
  bool LoadConfiguration();

  int port_;
  bool connected_;
  Configuration configuration_;
  ControllerVersion versions_;
};
} // namespace PS2Plus::App