#pragma once

#include "controller-configuration.h"
#include "util/firmware-update.h"

#include <string>

namespace PS2Plus::App {
enum ControllerUpdateStatus {
  kUpdateStatusNone,
  kUpdateStatusPending,
  kUpdateStatusRebooting,
  kUpdateStatusUpdating,
  kUpdateStatusUpdatePending,
  kUpdateStatusCompleted,
  kUpdateStatusFailed
};

struct ControllerUpdate {
  ControllerUpdateStatus status;
  FirmwareUpdate *firmware;
  int last_record_index;
  int total_records;
  float last_check_time;
};

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

  int port() const                      { return port_; }
  bool connected() const                { return connected_; }
  Configuration& configuration()        { return configuration_; }
  ControllerVersion& versions()         { return versions_; }
  ControllerUpdate& update()            { return update_; }

  bool Connect();

  void MockData();

private:
  bool LoadVersions();
  bool LoadConfiguration();

  int port_;
  bool connected_;
  Configuration configuration_;
  ControllerVersion versions_;
  ControllerUpdate update_;
};
} // namespace PS2Plus::App