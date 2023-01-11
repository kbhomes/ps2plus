#pragma once

#include <vector>

#include "app/controller.h"
#include "ui/gfx.h"
#include "ui/pad.h"

namespace PS2Plus::App {
class State {
public:
  State();
  State(const State&) = delete; // Disable the copy constructor so that all operations are always modifying the original instance

  void UpdateGamepads();
  
  void SelectController(int port);
  void MockController(int port);

  auto controllers() { return controllers_; }
  auto current_controller() { return current_controller_; }
  auto current_gamepad() { return current_gamepad_; }
  auto gamepad_summary() { return gamepad_summary_; }

  auto video_mode() { return video_mode_; }
  void set_video_mode(PS2Plus::Graphics::VideoMode mode);
  
private:
  std::vector<Controller *> controllers_;
  Controller *current_controller_;
  PS2Plus::Gamepad::PadStatus current_gamepad_;
  PS2Plus::Gamepad::PadSummary gamepad_summary_;
  PS2Plus::Graphics::VideoMode video_mode_ = PS2Plus::Graphics::Interlaced;
};

State& GetState();
} // namespace PS2Plus::App